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

#define MAX_HEIGHT 128
#define MAX_HEIGHT_IN_CHUNKS MAX_HEIGHT/16
#define MIN_HEIGHT 0

#define PLAYER_ACTION_RANGE 5


#define APROX_STEP_SIZE 0.1


// forward declaration
class SuperChunk;


void Update_Inventory_Items(Inventory* inv, InventoryMesh* invMesh, Font* font){
	int x=0;
	int y=0;
	invMesh->items.clearMesh();
	for (int i=0; i<9; i++){
		int id = inv->get_item(i+1)->getId();
		if (id!=0){
			ItemType* item = ItemTypeManager::GetItemType(id);
			glm::vec3 pos = calculate_slot_position(i);
			item->get_tex_coords(&x, &y);
			invMesh->items.addQuad(pos, 0, x, y, 0.11);
			int amount = inv->get_item(i+1)->getAmount();
			if (amount > 0){
				font->addLine(std::to_string(amount), pos.x * SDL_handler::getAspectRatio(), pos.y-0.06, 0.04);
			}
		}
	}
	invMesh->items.update();
}



class LocalPlayer
{
public:
	LocalPlayer(){
		//position = glm::vec3(8.0f, 8.0f, 25.0f);
		//model = glm::scale(model, glm::vec3(1.2f));
		camera.setFov(Config::getFloatValue("Fov"));
		camera.set_camera_height(player_height);
		camera.translate(glm::vec3(8.0f, 48.0f, -100.0f));
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



	virtual void translate(glm::vec3 v) {
		new_position += v;
	}
	void moveFront(float amount) {
		if (abs(vel_forward) < speed || sign(vel_forward) != sign(amount)){
			vel_forward += amount*15;
		}
		movement_input_f = true;
	}

	void moveSideways(float amount) {
		if (abs(vel_sideways) < speed || sign(vel_sideways) != sign(amount)){
			vel_sideways += amount*15;
		}
		movement_input_s = true;
	}

	void moveUp(float amount) {
		translate(up * amount * speed);
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

	void jump(){
		if (is_grounded){
			velocity.y = jump_strength;
		}
	}

	void move(float amount){
		if (length(velocity) > max_velocity){
			velocity = normalize(velocity) * max_velocity;
		}

		if(abs(vel_forward) > 0 && !movement_input_f && is_grounded){
			vel_forward *= 0.2;
		}
		translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * camera.getLookAt()) * amount * vel_forward);

		if(abs(vel_sideways) > 0 && !movement_input_s && is_grounded){
			vel_sideways *= 0.2;
		}
		translate(glm::normalize(glm::cross(camera.getLookAt(), up)) * amount * vel_sideways);

		glm::vec3 motion = new_position - position + velocity;
		glm::vec3 correct_motion = motion; 
		glm::vec3 pos = new_position + velocity;

		bool collided;
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
		collided = false;
		if (
			SuperChunk::has_Block_collision(glm::vec3(position.x + player_radius, pos.y + offset_y, position.z + player_radius)) ||
			SuperChunk::has_Block_collision(glm::vec3(position.x + player_radius, pos.y + offset_y, position.z - player_radius)) ||
			SuperChunk::has_Block_collision(glm::vec3(position.x - player_radius, pos.y + offset_y, position.z + player_radius)) ||
			SuperChunk::has_Block_collision(glm::vec3(position.x - player_radius, pos.y + offset_y, position.z - player_radius))){
			collided = true;
		}
		if (pos.y < MIN_HEIGHT || pos.y > MAX_HEIGHT){
			collided = true;
		}
		if (collided){
			if (motion.y <= 0){
				is_grounded = true;
			}
			correct_motion.y = 0.0f;
			pos.y = position.y -motion.y;
			velocity.y = -0.0;
		}




		// add is_gounded to x and z to add wall Climbing
		// check x
		collided = false;
		if (SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y, 						position.z + player_radius)) ||
			SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y, 						position.z - player_radius)) ||
			SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_half_height, position.z + player_radius)) ||
			SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_half_height, position.z - player_radius)) ||
			SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_height, 		position.z + player_radius)) ||
			SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_height, 		position.z - player_radius))){
			collided = true;
		}
		if (collided){
			//is_grounded = true;
			correct_motion.x = 0.0f;
			pos.x = position.x -motion.x;
		}


		

		// check z
		collided = false;
		if (SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius, pos.y, 					  pos.z + player_radius*sign_z)) ||
			SuperChunk::has_Block_collision(glm::vec3(pos.x - player_radius, pos.y, 					  pos.z + player_radius*sign_z)) ||
			SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius, pos.y + player_half_height, pos.z + player_radius*sign_z)) ||
			SuperChunk::has_Block_collision(glm::vec3(pos.x - player_radius, pos.y + player_half_height, pos.z + player_radius*sign_z)) ||
			SuperChunk::has_Block_collision(glm::vec3(pos.x + player_radius, pos.y + player_height, 	  pos.z + player_radius*sign_z)) ||
			SuperChunk::has_Block_collision(glm::vec3(pos.x - player_radius, pos.y + player_height, 	  pos.z + player_radius*sign_z))){
			collided = true;
		}
		if (collided){
			correct_motion.z = 0.0f;
			pos.z = position.z -motion.z;
		}

		camera.translate(correct_motion);
		update();

		movement_input_f = false;
		movement_input_s = false;
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
	const GLfloat* getModelViewProj_GL() {
		return (const GLfloat*)&modelViewProj[0][0];
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

	void updateLocalChunkIds() {
		chunksInSight.clear();

		for (int x = -sightDistance; x <= sightDistance; x++) {
			for (int y = -MAX_HEIGHT_IN_CHUNKS; y <= MAX_HEIGHT_IN_CHUNKS; y++) {
				for (int z = -sightDistance; z <= sightDistance; z++) {
					int pos_x = currentChunkPos.x + x * 16;
					int pos_y = currentChunkPos.y + y * 16;
					int pos_z = currentChunkPos.z + z * 16;
					if (pos_y >= MIN_HEIGHT && pos_y <= MAX_HEIGHT){
						chunksInSight.push_back(glm::vec3(pos_x, pos_y, pos_z));
					}
					
				}
			}
		}
	}

	std::vector<glm::vec3> getLocalChunkIds() {
		return chunksInSight;
	}

	void get_focussed_Block() {
		glm::vec3 lookAt = camera.getLookAt();
		std::cout << vec3_toString(lookAt, "lookAt") << std::endl;
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

	void place_block() {
		glm::vec3 lookAt = camera.getLookAt();
		std::cout << vec3_toString(lookAt, "lookAt") << std::endl;
		glm::vec3 stepPos;
		glm::vec3 viewpos = camera.getViewPos();
		glm::vec3 previousPos = viewpos;
		bool found = false;
		for (int i = 0; i * APROX_STEP_SIZE < PLAYER_ACTION_RANGE; i++) {
			float factor = i * APROX_STEP_SIZE;
			stepPos = viewpos + lookAt * factor; // multiplied Vector with scalar by using *. better change to function
			if (SuperChunk::getBlockTypeInt(stepPos)) {
				if (!block_collision(previousPos)){
					int block_type = inventory.get_item(selected_inventory_slot)->getId();
					inventory.pop_item(selected_inventory_slot);
					SuperChunk::setBlock(previousPos, block_type);
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
		skybox.render(position.x - player_radius, position.y + player_height-0.5, position.z - player_radius, getModelViewProj_GL());
		//skyQuad.draw();
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


private:
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 new_position;
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f); // Todo: Implement Velocity and Gravity
	float vel_forward = 0;
	float vel_sideways = 0;
	float max_velocity = 0.5;
	float jump_strength = 0.2;
	bool is_grounded = false;
	bool movement_input_f = false;
	bool movement_input_s = false;

	float player_radius = 0.4;
	float player_height =  1.8;
	float player_half_height = player_height/2;

	float speed = 8.0f;
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
	Skybox skybox = Skybox("shaders/skybox.vs", "shaders/skybox.fs", "graphics/HA_logo_saved.jpg");
	Quad skyQuad = Quad();
	glm::vec3 selection_box_pos;
	bool selection_box_is_focussed = false;

	// more WIP --- INVENTORY
	//int selected_block_type = 4;
	int selected_inventory_slot = 4;
	Inventory inventory;

};



