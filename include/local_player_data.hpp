#pragma once

#include <cmath>
#include <vector>
#include <ostream>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "floating_camera.hpp"
#include "printFunktions.hpp"
#include "util_funcs.hpp"

#include "object_models.hpp"

#include "SuperChunk.hpp"

#include "item.hpp"
#include "inventory.hpp"
#include "text.hpp"
#include "SDL_handler.hpp"
#include <string>


#include "config.hpp"

#include "world.hpp"


#define MAX_HEIGHT 128
#define MAX_HEIGHT_IN_CHUNKS MAX_HEIGHT/16
#define MIN_HEIGHT 0

#define PLAYER_ACTION_RANGE 5


#define APROX_STEP_SIZE 0.1


// forward declaration
class SuperChunk;



enum PlayerMovementState {Walking, Flying, Swimming};
enum JumpMethod {SingleJump, DoubleJump, Fly};

//std::ostream& operator << ( std::ostream& outs, const PlayerMovementState val)
//{
//    std::string s;
//    if (val == Walking){
//        s = "Walking";
//    }else if (val == Flying){
//            s = "Flying";
//    }else if (val == Swimming){
//        s = "Swimming";
//    }else {
//        s = "Undefined";
//    }
//    s = "PlayerMovementState: " + s;
//return outs << s;
//}
//
//std::ostream& operator << ( std::ostream& outs, const JumpMethod val)
//{
//    std::string s;
//    if (val == SingleJump){
//        s = "SingleJump";
//    }else if (val == DoubleJump){
//        s = "DoubleJump";
//    }else if (val == Fly){
//        s = "Fly";
//    }else {
//        s = "Undefined";
//    }
//    s = "JumpMethod: " + s;
//    return outs << s;
//}


class LocalPlayer
{
public:
    LocalPlayer(){
        //position = glm::vec3(8.0f, 8.0f, 25.0f);
        //model = glm::scale(model, glm::vec3(1.2f));
        fov = Config::getFloatValue("Fov");
        camera.setFov(fov);
        camera.set_camera_height(player_height);
        camera.translate(World::getvec3Value("PlayerPosition"));
        glm::vec3 yaw_pitch = World::getvec3Value("PlayerLookAt");
        yaw = yaw_pitch.x;
        pitch = yaw_pitch.y;
        lookAt = clacLookAt(yaw, pitch);
        camera.setLookAt(lookAt);

        chunksInSight.reserve(sightDistance * sightDistance * sightDistance);
        update();
        lastChunkPos = currentChunkPos;
    }
    ~LocalPlayer() {}

    void update() {
        //camera.translate(position);
        position = camera.getPos();
        new_position = position;
        camera.update();
        calculateChunkPos();
        if (currentChunkPos != lastChunkPos) {
            //std::cout << "has changed" << std::endl;
            changedChunk = true;
            lastChunkPos = currentChunkPos;
            updateLocalChunkIds();
        }
        modelViewProj = camera.getViewProj() * model;
    }

    void calculateChunkPos() {
        currentChunkPos = Chunk_Position(position);
    }

    bool hasChangedChunk() {
        if (changedChunk) {
            changedChunk = false;
            return true;
        }
        else {
            return false;
        }
    }

    void onMouseMoved(float xRel, float yRel) {
        yaw += xRel * mouseSensitivity;
        pitch -= yRel * mouseSensitivity;
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
        lookAt = clacLookAt(yaw, pitch);
        camera.setLookAt(lookAt);
        camera.update();
    }

    virtual void translate(glm::vec3 v) {
        new_position += v;
    }
    void moveFront(float amount) { // Todo: rewrite this again
//        if(is_sprinting && abs(velocity.z) < speed*sprintingFactor || sign(velocity.z) != sign(amount)){
//            velocity.z += amount*15;
//        }else if (abs(velocity.z) < speed || sign(velocity.z) != sign(amount)){
//            velocity.z += amount*15;
//        }

        //velocity += lookAt * amount * 3.0f; // for dynamic flying e.g. Jetpack

        if (is_sprinting){
            amount *= sprintingFactor;
        }

        if(is_grounded){
            velocity += normalize(onlyXZ(lookAt)) * amount;
        }else{
            velocity +=  normalize(onlyXZ(lookAt)) * amount * 0.1f;
        }
        movement_input_f = true;
    }


    void moveSideways(float amount) {
        if (is_sprinting){
            amount *= sprintingFactor;
        }

        if(is_grounded){
            velocity +=  normalize(glm::vec3(-lookAt.z, 0, lookAt.x)) * amount;
        }else{
            velocity +=  normalize(glm::vec3(-lookAt.z, 0, lookAt.x)) * amount*0.1f;
        }
        movement_input_s = true;
    }


    void moveUp(float amount) {
        velocity +=  up * amount * speedFactorUp;
        movement_input_u = true;
    }


    void decelFront(float decel_fac) { // Todo: rewrite this again
        auto rotationMat_l_to_g = glm::rotate(glm::mat4(1.0f), -vectorAngleRadXZ(lookAt.x, lookAt.z), {0, 1, 0});
        auto rotationMat_g_to_l = glm::rotate(glm::mat4(1.0f), vectorAngleRadXZ(lookAt.x, lookAt.z), {0, 1, 0});
        auto g_vel = glm::vec3(rotationMat_l_to_g * glm::vec4(velocity, 1.0));
        g_vel.z *= decel_fac;
        velocity = glm::vec3(rotationMat_g_to_l * glm::vec4(g_vel, 1.0));

    }

    void decelSideways(float decel_fac) { // Todo: rewrite this again
        auto rotationMat_l_to_g = glm::rotate(glm::mat4(1.0f), -vectorAngleRadXZ(lookAt.x, lookAt.z), {0, 1, 0});
        auto rotationMat_g_to_l = glm::rotate(glm::mat4(1.0f), vectorAngleRadXZ(lookAt.x, lookAt.z), {0, 1, 0});
        auto g_vel = glm::vec3(rotationMat_l_to_g * glm::vec4(velocity, 1.0));
        g_vel.x *= decel_fac;
        velocity = glm::vec3(rotationMat_g_to_l * glm::vec4(g_vel, 1.0));

    }

    void decelUp(float decel_fac) { // Todo: rewrite this again
        velocity.y *= decel_fac;
        movement_input_u = true;
    }


    // the force should be a normalized vector
    void add_force(float delta_time, glm::vec3 force){
        velocity += force *  delta_time;
    }

    void apply_gravity(float delta_time, glm::vec3 force){
        if (!is_grounded){
            velocity += force *  delta_time;
        }
        else{
            if (velocity.y < 0){
                //velocity.y = -0.0; // why?
            }
        }
    }

/* 	bool test_block_collision(int x, int y, int z){
		float dist_x = abs(position.x - (x + 0.5));
		float dist_y = abs(position.y + player_half_height - (y + 0.5));
		float dist_z = abs(position.z - (z + 0.5));
		if (dist_x > 0.6 + player_radius ||
			dist_y > 1 + player_half_height ||
			dist_z > 0.6 + player_radius){
				return false;
			}
		else{
			return true;
		}
	}  */

    void jump(bool secondJump=false, double amount=0.0f){
        std::cout << "Jump" << std::endl;
        switch (jumpMethod) {
            case SingleJump:
                if (is_grounded){
                    velocity.y = jump_strength;
                }
                break;
            case DoubleJump:
                if (is_grounded){
                    std::cout << "is_grounded" << std::endl;
                    velocity.y = jump_strength;
                    second_jump_allowed = true;
                }else if(secondJump and second_jump_allowed){

                    std::cout << "secondJump" << std::endl;
                    velocity.y += jump_strength;
                    second_jump_allowed = false;
                }
                break;
            case Fly:
                if (movementState == Walking){
                    if (is_grounded){
                        std::cout << "Fly Jump is_grounded" << std::endl;
                        velocity.y = jump_strength;
                        second_jump_allowed = true;
                    }else if(secondJump){
                        std::cout << "Flx Jump secondJump" << std::endl;
                        velocity.y = jump_strength;
                        movementState = Flying;
                    }
                }else if (movementState == Flying){
                    if(secondJump){
                        movementState = Walking;
                    }else{
                        std::cout << "Fly up" << std::endl;
                        moveUp(amount);
                        //velocity +=  up * amount;
                    }
                }

                break;
        }
        ticks_since_last_jump = 0;
    }

    bool check_collision_y(glm::vec3 pos, float offset_y){
        return (
                SuperChunk::has_Block_collision(glm::vec3(position.x + player_radius, pos.y + offset_y, position.z + player_radius)) ||
                SuperChunk::has_Block_collision(glm::vec3(position.x + player_radius, pos.y + offset_y, position.z - player_radius)) ||
                SuperChunk::has_Block_collision(glm::vec3(position.x - player_radius, pos.y + offset_y, position.z + player_radius)) ||
                SuperChunk::has_Block_collision(glm::vec3(position.x - player_radius, pos.y + offset_y, position.z - player_radius)) ||
                pos.y < MIN_HEIGHT || pos.y > MAX_HEIGHT
        );
    }

    bool check_collision_x(glm::vec3 pos, float sign_x){
        return (
                SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y, 						position.z + player_radius)) ||
                SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y, 						position.z - player_radius)) ||
                SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_half_height, position.z + player_radius)) ||
                SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_half_height, position.z - player_radius)) ||
                SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_height, 		position.z + player_radius)) ||
                SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_height, 		position.z - player_radius))
        );
    }

    bool check_collision_z(glm::vec3 pos, float sign_z){
        return (
                SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius, pos.y, 					  pos.z + player_radius*sign_z)) ||
                SuperChunk::has_Block_collision(glm::vec3(pos.x - player_radius, pos.y, 					  pos.z + player_radius*sign_z)) ||
                SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius, pos.y + player_half_height, pos.z + player_radius*sign_z)) ||
                SuperChunk::has_Block_collision(glm::vec3(pos.x - player_radius, pos.y + player_half_height, pos.z + player_radius*sign_z)) ||
                SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius, pos.y + player_height, 	  pos.z + player_radius*sign_z)) ||
                SuperChunk::has_Block_collision(glm::vec3(pos.x - player_radius, pos.y + player_height, 	  pos.z + player_radius*sign_z))
        );
    }

    void move(float amount){

        if (movementState == Walking){
            if(is_sprinting){
                std::cout << "is Sprinting" << std::endl;
                velocity = limitXZ(velocity, max_velocity*sprintingFactor);
            }else{
                velocity = limitXZ(velocity, max_velocity);
            }
        }
        else if (movementState == Flying){
            if(is_sprinting){
                velocity = limitXZ(velocity, maxFlightSpeed*sprintingFactor);
            }else{
                velocity = limitXZ(velocity, maxFlightSpeed);
            }

            if (abs(velocity.y) > maxFlightSpeed){
                velocity.y = maxFlightSpeed * (float)sign(velocity.y);
            }
        }


        float vel_xz_ratio = angleXZ(velocity) / 90.0f;

//        if (is_grounded || (!movement_input_f && !movement_input_s)){
//            velocity = scaleXZ(velocity, 0.5);
//        }

        float decel_fac;
        if (is_grounded){
            if(movementState == Flying){
                movementState = Walking;
            }
            decel_fac = 0.2;
        }
        else{
            decel_fac = 0.9;
        }

        if (!movement_input_f){
            decelFront(decel_fac);
        }
        if (!movement_input_s){
            decelSideways(decel_fac);
        }
        if(movementState == Flying && !movement_input_u){
            decelUp(0.8);
        }

        translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * camera.getLookAt()) * amount * velocity.z);
        translate(glm::normalize(glm::cross(camera.getLookAt(), up)) * amount * velocity.x);

        glm::vec3 motion = new_position - position + velocity;
        glm::vec3 correct_motion = motion;
        glm::vec3 pos = new_position + velocity;
        glm::vec3 lookAt = camera.getLookAt();

        int sign_x = sign(motion.x);
        int sign_z = sign(motion.z);
        float offset_y;
        if (motion.y>0){
            offset_y = player_height;
        }
        else{
            offset_y = 0;
        }

        // check y
        is_grounded = false;
        if (check_collision_y(pos, offset_y)){
            if (motion.y <= 0){
                is_grounded = true;
            }
            correct_motion.y = 0.0f;
            pos.y = position.y -motion.y;
            velocity.y = -0.0;
        }


        // add is_gounded to x and z to add wall Climbing
        // check x

        if (check_collision_x(pos, sign_x)){
            //is_grounded = true;
            correct_motion.x = 0.0f;
            pos.x = position.x -motion.x;
            velocity.x = 0;
        }


        // check z
        if (check_collision_z(pos, sign_z)){
            correct_motion.z = 0.0f;
            pos.z = position.z -motion.z;
            velocity.z = 0;
        }

        camera.translate(correct_motion);
        update();

        movement_input_f = false;
        movement_input_s = false;
        movement_input_u = false;
        ticks_since_last_jump++;
    }


    bool block_collision(glm::vec3 blockpos){
        blockpos = floor(blockpos);

        if (is_inside_block(blockpos, glm::vec3(position.x + player_radius, position.y,				         position.z + player_radius)) ||
            is_inside_block(blockpos, glm::vec3(position.x + player_radius, position.y,						 position.z - player_radius)) ||
            is_inside_block(blockpos, glm::vec3(position.x - player_radius, position.y,						 position.z + player_radius)) ||
            is_inside_block(blockpos, glm::vec3(position.x - player_radius, position.y, 					 position.z - player_radius)) ||

            is_inside_block(blockpos, glm::vec3(position.x + player_radius, position.y + player_half_height, position.z + player_radius)) ||
            is_inside_block(blockpos, glm::vec3(position.x + player_radius, position.y + player_half_height, position.z - player_radius)) ||
            is_inside_block(blockpos, glm::vec3(position.x - player_radius, position.y + player_half_height, position.z + player_radius)) ||
            is_inside_block(blockpos, glm::vec3(position.x - player_radius, position.y + player_half_height, position.z - player_radius)) ||

            is_inside_block(blockpos, glm::vec3(position.x + player_radius, position.y + player_height,      position.z + player_radius)) ||
            is_inside_block(blockpos, glm::vec3(position.x + player_radius, position.y + player_height,      position.z - player_radius)) ||
            is_inside_block(blockpos, glm::vec3(position.x - player_radius, position.y + player_height, 	 position.z + player_radius)) ||
            is_inside_block(blockpos, glm::vec3(position.x - player_radius, position.y + player_height,      position.z - player_radius))){
            return true;
        }
        else{
            return false;
        }


    }

    //Get / Set Fuctions,  some could be removed later
    FloatingCamera* getCamera() {
        return &camera;
    }
    glm::vec3 getCurrentChunkPos() {
        return currentChunkPos;
    }
    glm::vec3 getPosition() {
        return position;
    }
    glm::mat4 getModelViewProj() {
        return modelViewProj;
    }
    GLfloat* getModelViewProj_GL() {
        return (GLfloat*)&modelViewProj[0][0];
    }

    void printLocalChunks() {
        std::stringstream ss;
        string output;
        glm::vec3 chunkPosition;
        int counter = 0;
        for (int x = -sightDistance; x <= sightDistance; x++) {
            for (int y = -sightDistance; y <= sightDistance; y++) {
                for (int z = -sightDistance; z <= sightDistance; z++) {
                    chunkPosition.x = currentChunkPos.x + x * 16;
                    chunkPosition.y = currentChunkPos.y + y * 16;
                    chunkPosition.z = currentChunkPos.z + z * 16;
                    ss << vec3_toString(chunkPosition) << "  ";
                    counter++;
                }
                ss << "\n";
            }
            ss << "\n";
        }
        output = ss.str();
        std::cout << output << std::endl;
    }
    void _addChunkStack(int pos_x, int pos_z){
        for (int y = -MAX_HEIGHT_IN_CHUNKS; y <= MAX_HEIGHT_IN_CHUNKS; y++) {
            int pos_y = currentChunkPos.y + y * 16; // might cause problems
            if (pos_y >= MIN_HEIGHT && pos_y <= MAX_HEIGHT){
                chunksInSight.push_back(glm::vec3(pos_x, pos_y, pos_z));
            }
        }
    }

    void _addCunkline_x(int pos_z, int x_start, int x_end){
        if (x_start > x_end){ // use swap function instead
            int dummy = x_end;
            x_end = x_start;
            x_start = dummy;
        }
        for (int i=0; i < (x_end-x_start)/16; i++){
            int pos_x = x_start + i * 16;
            _addChunkStack(pos_x, pos_z);
        }
    }

    void _addCunkline_z(int pos_x, int z_start, int z_end){
        if (z_start > z_end){ // use swap function instead
            int dummy = z_end;
            z_end = z_start;
            z_start = dummy;
        }
        for (int i=0; i < (z_end-z_start)/16; i++){
            int pos_z = z_start + i * 16;
            _addChunkStack(pos_x, pos_z);
        }
    }

    void updateLocalChunkIds() {
        chunksInSight.clear();

        int base_x = currentChunkPos.x;
        int base_z = currentChunkPos.z;

        _addChunkStack(base_x, base_z);

        for (int z=1; z<=sightDistance; z++){
            _addChunkStack(base_x, base_z + 16*z);
            _addChunkStack(base_x, base_z - 16*z);
        }

        for (int x=1; x<=sightDistance; x++){
            int offset_x = 16*x;
            _addChunkStack(base_x + offset_x, base_z);
            _addChunkStack(base_x - offset_x, base_z);
            for (int z=1; z<=sightDistance; z++){
                _addChunkStack(base_x + offset_x, base_z + 16*z);
                _addChunkStack(base_x + offset_x, base_z - 16*z);
                _addChunkStack(base_x - offset_x, base_z + 16*z);
                _addChunkStack(base_x - offset_x, base_z - 16*z);
            }
        }
    }

    std::vector<glm::vec3> getLocalChunkIds() {
        return chunksInSight;
    }

    void get_focussed_Block() {
        glm::vec3 lookAt = camera.getLookAt();
        //std::cout << vec3_toString(lookAt, "lookAt") << std::endl;
        glm::vec3 stepPos;
        glm::vec3 viewpos = camera.getViewPos();
        bool found = false;
        for (int i = 0; i * APROX_STEP_SIZE < PLAYER_ACTION_RANGE; i++) {
            float factor = i * APROX_STEP_SIZE;
            stepPos = viewpos + lookAt * factor; // multiplied Vector with scalar by using *. better change to function
            if (SuperChunk::getBlockTypeInt(stepPos)) {
                int block_to_remove = SuperChunk::getBlockTypeInt(stepPos);
                inventory.add_item(block_to_remove, 1);
                SuperChunk::setBlock(stepPos, 0);
                found = true;
                //selection_box_pos = floor(stepPos);
                break;
            }
        }
        update_selection_box();
    }

    BlockDirection calc_block_placing_rotation(glm::vec3 place, glm::vec3 adjacent){
        int x1,y1,z1,x2,y2,z2;
        x1 = place.x;
        y1 = place.y;
        z1 = place.z;

        x2 = adjacent.x;
        y2 = adjacent.y;
        z2 = adjacent.z;

        BlockDirection rotation;

        if (x1-x2 < 0){
            rotation = X_negative;
            std::cout << "-x" << std::endl;
        }else if (x1-x2 > 0){
            rotation = X_positive;
            std::cout << "+x" << std::endl;
        }else if (y1-y2 < 0){
            rotation = Y_negative;
            std::cout << "-y" << std::endl;
        }else if (y1-y2 > 0){
            rotation = Y_positive;
            std::cout << "+y" << std::endl;
        }else if (z1-z2 < 0){
            rotation = Z_negative;
            std::cout << "-z" << std::endl;
        }else if (z1-z2 > 0){
            rotation = Z_positive;
            std::cout << "+z" << std::endl;
        }else{
            rotation = Z_positive;
            std::cout << "Rotation error" << std::endl;
        }

        return rotation;
    }

    void place_block() {
        glm::vec3 lookAt = camera.getLookAt();
        std::cout << vec3_toString(lookAt, "lookAt") << std::endl;
        glm::vec3 stepPos;
        glm::vec3 viewpos = camera.getViewPos();
        glm::vec3 previousPos = viewpos;
        bool found = false;
        int block_type = inventory.get_item(selected_inventory_slot)->getId();
        for (int i = 0; i * APROX_STEP_SIZE < PLAYER_ACTION_RANGE; i++) {
            float factor = i * APROX_STEP_SIZE;
            stepPos = viewpos + lookAt * factor; // multiplied Vector with scalar by using *. better change to function
            if ( SuperChunk::getBlockTypeInt(stepPos)) {
                BlockType* bt = BlockTypeManager::GetBlockType(block_type);

                if (!block_collision(previousPos) || !bt->hasCollision()){
                    if (!bt->isPlaceableOnAir()){
                        //TODO: SuperChunk needs to be fixed for this to work - should be fixed now (27.12.24)
                        if (!SuperChunk::has_Block_collision(previousPos - glm::vec3(0,0.99,0))){
                            break;
                        }
                    }


                    if (bt->get_rotation_type() != None){
                        BlockDirection rot = calc_block_placing_rotation(previousPos, stepPos);
                        SuperChunk::setBlock(previousPos, block_type, rot);
                    }else{
                        SuperChunk::setBlock(previousPos, block_type);
                    }
                    inventory.pop_item(selected_inventory_slot);

                    found = true;
                }
                break;
            }
            previousPos = stepPos;
        }
        update_selection_box();
    }

    int get_sight_distance(){
        return sightDistance;
    }

    // Work in Progress Functions:
    // --- RENDERING ---

    /* void set_selected_block_type(int id){
        selected_block_type = id;
    }
    int get_selected_block_type(){
        return selected_block_type;
    } */

    void render_selection_box(){
        if (selection_box_is_focussed){
            selection_box.render(selection_box_pos.x, selection_box_pos.y, selection_box_pos.z, getModelViewProj_GL());
        }

    }
    void update_selection_box(){
        glm::vec3 lookAt = camera.getLookAt();
        glm::vec3 stepPos;
        glm::vec3 viewpos = camera.getViewPos();
        bool found = false;
        for (int i = 0; i * APROX_STEP_SIZE < PLAYER_ACTION_RANGE; i++) {
            float factor = i * APROX_STEP_SIZE;
            stepPos = viewpos + lookAt * factor; // multiplied Vector with scalar by using *. better change to function
            if (SuperChunk::getBlockTypeInt(stepPos)) {
                selection_box_pos = floor(stepPos);
                found = true;
                selection_box_is_focussed = true;
                break;
            }
        }
        if (!found){
            selection_box_is_focussed = false;
        }
    }

    void render_skybox(){
        if (use_fancy_sky){
            //skyQuad.render(camera.getLookAt());
            glm::mat4 inv_projection = glm::inverse(camera.getProjection());
            glm::mat4 inv_view = glm::inverse(camera.getView());
            skyQuad.render(camera.getRotation(),
                           (const GLfloat*)&inv_projection[0][0],
                           (const GLfloat*)&inv_view[0][0]);
        }
        else{
            skybox.render(position.x - player_radius, position.y + player_height-0.5, position.z - player_radius, getModelViewProj_GL());
        }
    }

    // glm::vec3 getLookAt(){
    // 	return camera.getLookAt();
    // }

    void zoomIn(bool should_zoom){
        if (zoom != should_zoom){
            if (should_zoom){
                camera.setFov(zoomFov);
            }else{
                camera.setFov(fov);
            }
            zoom = should_zoom;
        }
    }



    // --- INVENTORY ---

    void debug_print_inventory(){
        inventory.printInfo();
    }

    std::string inventory_as_string(){
        //return inventory.info_string();
        return inventory.improved_info_string(selected_inventory_slot);
    }

    void set_inventory_slot(int slot){
        selected_inventory_slot = slot;
    }

    int get_inventory_slot(){
        return selected_inventory_slot;
    }
    int get_selected_item_type(){
        return inventory.get_item(selected_inventory_slot)->getId();
    }
    Inventory* get_inventory(){
        return &inventory;
    }

    void Update_Inventory_Items(InventoryMesh* invMesh, Font* font){
        int x=0;
        int y=0;
        invMesh->items.clearMesh();
        for (int i=0; i<9; i++){
            int id = inventory.get_item(i+1)->getId();
            if (id!=0){
                ItemType* item = ItemTypeManager::GetItemType(id);
                glm::vec3 pos = calculate_slot_position(i);
                item->get_tex_coords(&x, &y);
                invMesh->items.addQuad(pos, 0, x, y, 0.11);
                int amount = inventory.get_item(i+1)->getAmount();
                if (amount > 0){
                    font->addLine(std::to_string(amount), pos.x * SDL_handler::getAspectRatio(), pos.y-0.06, 0.04);
                }
            }
        }
        invMesh->items.update();
    }



    PlayerMovementState getMovementState(){
        return movementState;
    }

    JumpMethod getJumpMethod(){
        return jumpMethod;
    }

    bool getIsSprinting(){
        return is_sprinting;
    }

    void setIsSprinting(bool val){
        is_sprinting = val;
    }

    glm::vec3 getVelocity(){
        return velocity;
    }

    glm::vec3 getSelectionPos(){
        return selection_box_pos;
    }



    //////////////////////////
    // --- World Saving --- //
    //////////////////////////

    void setPlayerValues_inWorldData(){
        World::setvec3Value("PlayerPosition", position);
        World::setvec3Value("PlayerLookAt", camera.getRotation());
    }

private:
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 new_position;
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f); // Todo: Implement Velocity and Gravity

    float yaw;
    float pitch;
    glm::vec3 lookAt;
    float mouseSensitivity = 0.3f;
    float fov;
    float zoomFov = 88.7;
    bool zoom = false;

    //float vel_forward = 0;
    //float vel_sideways = 0;
    float max_velocity = 0.08;
    float jump_strength = 0.15;
    bool is_grounded = false;
    bool movement_input_f = false;
    bool movement_input_s = false;
    bool movement_input_u = false;

    PlayerMovementState movementState = Walking;
    JumpMethod jumpMethod = Fly;
    bool is_sprinting = false;
    float sprintingFactor = 2;

    float player_radius = 0.3;
    float player_height =  1.6;
    float player_half_height = player_height/2;

    float speedFactorUp = 0.3f;
    float maxFlightSpeed = 0.2;
    int sightDistance = Config::getIntValue("SightDistance");
    std::vector<glm::vec3> chunksInSight;
    glm::vec3 position;
    glm::vec3 currentChunkPos;
    glm::vec3 lastChunkPos;
    bool changedChunk = true;

    FloatingCamera camera = *new FloatingCamera();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 modelViewProj = camera.getViewProj() * model;




    // still WIP --- RENDERING

    Box selection_box = Box("shaders/selection_box.vs", "shaders/selection_box.fs", "graphics/selection_box_64_2.png");
    glm::vec3 selection_box_pos;
    bool selection_box_is_focussed = false;

    bool use_fancy_sky = Config::getBoolValue("FancySky");
    Skybox skybox = Skybox("shaders/skybox.vs", "shaders/skybox.fs", "graphics/HA_logo_saved.jpg");
    SkyQuad skyQuad = SkyQuad("shaders/sky.vs", "shaders/sky.fs");


    // more WIP --- INVENTORY
    //int selected_block_type = 4;
    int selected_inventory_slot = 4;
    Inventory inventory;

    int ticks_since_last_jump=0;
    bool second_jump_allowed = false;
};



