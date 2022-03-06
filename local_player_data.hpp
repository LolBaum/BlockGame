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

#include "SuperChunk.hpp"

#define MAX_HEIGHT 128
#define MIN_HEIGHT 0

#define PLAYER_ACTION_RANGE 5


#define APROX_STEP_SIZE 0.1


class LocalPlayer
{
public:
	LocalPlayer(){
		//position = glm::vec3(8.0f, 8.0f, 25.0f);
		//model = glm::scale(model, glm::vec3(1.2f));
		camera.set_camera_height(player_height);
		camera.translate(glm::vec3(8.0f, 28.0f, -100.0f));
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
		//translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * camera.getLookAt()) * amount * vel_forward);
		std::cout << "amount " << amount << std::endl; 
		if (abs(vel_forward) < speed || sign(vel_forward) != sign(amount)){
			vel_forward += amount*15;
		}
		std::cout << "vel_forward " << vel_forward << std::endl; 
		movement_input = true;
	}
	void moveSideways(float amount) {
		//translate(glm::normalize(glm::cross(camera.getLookAt(), up)) * amount * speed);
		if (abs(vel_sideways) < speed || sign(vel_sideways) != sign(amount)){
			vel_sideways += amount*15;
		}
		movement_input = true;
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
				velocity = force * delta_time;
			}
			
		}
	}

/* 	bool test_block_collision(int x, int y, int z){
		float dist_x = position.x - (x + 0.5);
		float dist_y = position.y + player_height - (y + 0.5);
		float dist_z = position.z - (z + 0.5);
		if (dist_x < 0.5 + player_radius ||
			dist_y < 0.5 + player_height/2.0 ||
			dist_z < 0.5 + player_radius){
				return true;
			}
		else{
			return false;
		}
	} */

	void jump(){
		if (is_grounded){
			velocity.y = jump_strength;
		}
	}

	void move(float amount){
		if (length(velocity) > max_velocity){
			velocity = normalize(velocity) * max_velocity;
		}
		if(abs(vel_forward) > 0 && !movement_input && is_grounded){
			vel_forward *= 0.5;
		}
		translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * camera.getLookAt()) * amount * vel_forward);

		if(abs(vel_sideways) > 0 && !movement_input && is_grounded){
			vel_sideways *= 0.5;
		}
		translate(glm::normalize(glm::cross(camera.getLookAt(), up)) * amount * vel_sideways);
		//std::cout << vec3_toString(velocity) << std::endl; 
		glm::vec3 motion = new_position - position + velocity;
		//std::cout << vec3_toString(motion, "amotion") << std::endl; 
/* 		if (! is_grounded || velocity.y > 0){
			motion.y += velocity.y;
		} */
		//motion.y += velocity.y;
		//std::cout << vec3_toString(motion, "bmotion") << std::endl; 
		glm::vec3 correct_motion = motion; 
		glm::vec3 pos = new_position + velocity;
		//std::cout << vec3_toString(motion, "motion") << std::endl; 

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
/* 		if (velocity.y <=0.001 && chunkManager.getBlockTypeInt(glm::vec3(position.x, pos.y-0.0001, position.z))){
			is_grounded = true;
		} */
		collided = false;
		if (chunkManager.getBlockTypeInt(glm::vec3(position.x + player_radius * sign_x, pos.y + offset_y, position.z + player_radius)) ||
			chunkManager.getBlockTypeInt(glm::vec3(position.x + player_radius * sign_x, pos.y + offset_y, position.z - player_radius)) ||
			chunkManager.getBlockTypeInt(glm::vec3(position.x + player_radius, 		    pos.y + offset_y, position.z + player_radius*sign_z)) ||
			chunkManager.getBlockTypeInt(glm::vec3(position.x - player_radius,          pos.y + offset_y, position.z + player_radius*sign_z))){
			collided = true;
		}
		if (collided){
			is_grounded = true;
			//velocity = glm::vec3(velocity.x, velocity.y, velocity.z);
			correct_motion.y = 0.0f;
			pos.y = position.y -motion.y;
		}
		//std::cout << is_grounded << std::endl; 


		// add is_gounded to x and z to add wall Climbing
		// check x
		//is_grounded = false;
		collided = false;
		if (chunkManager.getBlockTypeInt(glm::vec3(pos.x + player_radius * sign_x, pos.y, 						position.z + player_radius)) ||
			chunkManager.getBlockTypeInt(glm::vec3(pos.x + player_radius * sign_x, pos.y, 						position.z - player_radius)) ||
			chunkManager.getBlockTypeInt(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_half_height, position.z + player_radius)) ||
			chunkManager.getBlockTypeInt(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_half_height, position.z - player_radius)) ||
			chunkManager.getBlockTypeInt(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_height, 		position.z + player_radius)) ||
			chunkManager.getBlockTypeInt(glm::vec3(pos.x + player_radius * sign_x, pos.y + player_height, 		position.z - player_radius))){
			collided = true;
		}
		if (collided){
			//is_grounded = true;
			correct_motion.x = 0.0f;
			pos.x = position.x -motion.x;
		}


		

		// check z
		collided = false;
		if (chunkManager.getBlockTypeInt(glm::vec3(pos.x + player_radius, pos.y, 					  pos.z + player_radius*sign_z)) ||
			chunkManager.getBlockTypeInt(glm::vec3(pos.x - player_radius, pos.y, 					  pos.z + player_radius*sign_z)) ||
			chunkManager.getBlockTypeInt(glm::vec3(pos.x + player_radius, pos.y + player_half_height, pos.z + player_radius*sign_z)) ||
			chunkManager.getBlockTypeInt(glm::vec3(pos.x - player_radius, pos.y + player_half_height, pos.z + player_radius*sign_z)) ||
			chunkManager.getBlockTypeInt(glm::vec3(pos.x + player_radius, pos.y + player_height, 	  pos.z + player_radius*sign_z)) ||
			chunkManager.getBlockTypeInt(glm::vec3(pos.x - player_radius, pos.y + player_height, 	  pos.z + player_radius*sign_z))){
			collided = true;
		}
		if (collided){
			correct_motion.z = 0.0f;
			pos.z = position.z -motion.z;
		}

		camera.translate(correct_motion);
		update();

		movement_input = false;
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
			for (int y = -sightDistance; y <= sightDistance; y++) {
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
			if (chunkManager.getBlockTypeInt(stepPos)) {
				chunkManager.setBlock(stepPos, 0);
				found = true;
				break;
			}
		}
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
			if (chunkManager.getBlockTypeInt(stepPos)) {
				if (glm::length(viewpos - previousPos) > 1.4 &&
				    floor(previousPos) != floor(position) ) {
					chunkManager.setBlock(previousPos, 1);
					found = true;
				}
				break;
			}
			previousPos = stepPos;
		}
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
	bool movement_input = false;

	float player_radius = 0.3;
	float player_height =  1.8;
	float player_half_height = player_height/2;

	float speed = 8.0f;
	int sightDistance = 5;
	std::vector<glm::vec3> chunksInSight;
	glm::vec3 position;
	glm::vec3 currentChunkPos;
	glm::vec3 lastChunkPos;
	bool changedChunk = true;

	FloatingCamera camera = *new FloatingCamera();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 modelViewProj = camera.getViewProj() * model;
};



