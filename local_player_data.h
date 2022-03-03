#pragma once

#include <cmath>
#include <vector>
#include <ostream>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "floating_camera.h"
#include "printFunktions.h"
#include "util_funcs.h"

#include "SuperChunk.h"

#define MAX_HEIGHT 128
#define MIN_HEIGHT 0

#define PLAYER_ACTION_RANGE 5
#define PLAYER_RADIUS 0.3
#define PLAYER_HEIGHT 1.8

#define APROX_STEP_SIZE 0.1


class LocalPlayer
{
public:
	LocalPlayer(){
		//position = glm::vec3(8.0f, 8.0f, 25.0f);
		//model = glm::scale(model, glm::vec3(1.2f));
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
		translate(glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) * camera.getLookAt()) * amount);
	}
	void moveSideways(float amount) {
		translate(glm::normalize(glm::cross(camera.getLookAt(), up)) * amount);
	}
	void moveUp(float amount) {
		translate(up * amount);
	}

	void move(){
		/* glm::vec3 motion = new_position - position;
		glm::vec3 correct_motion = glm::vec3(0,0,0);
		glm::vec3 pos = position;
		int xMin = floorf(new_position.x - PLAYER_RADIUS);
		int xMax = floorf(new_position.x + PLAYER_RADIUS);
		int yMin = floorf(new_position.y);
		int yMax = floorf(new_position.y + PLAYER_HEIGHT);
		int zMin = floorf(new_position.z - PLAYER_RADIUS);
		int zMax = floorf(new_position.z + PLAYER_RADIUS);
		int new_x, new_y, new_z;
		bool testX = false;
    	bool testY = false;
    	bool testZ = false;
		bool collided = false;

		//std::cout << "Block at Pos: " << chunkManager.getBlockTypeInt(new_position)<< std::endl;
		//std::cout << "x: " << xMin << " " << xMax << ", y: " << yMin << " " << yMax << ", z: " << zMin << " " << zMax << std::endl;
		

		if(motion.x < 0){
			new_x = floorf(pos.x + motion.x - PLAYER_RADIUS);
			testX = true;
		}
		else if(motion.x > 0){
			new_x = floorf(pos.x + motion.x + PLAYER_RADIUS);
			testX = true;
		}

		

		if(testX){
			collided = false;
			for (int y=yMin; y<=yMax; y++){
				for (int z=zMin; z<=zMax; z++){
					// std::cout << "Block at Pos "<< vec3_toString(new_position) <<" : " << chunkManager.getBlockTypeInt(new_position)<< std::endl;
					// std::cout << "pos for  X: " << vec3_toString(glm::vec3(new_x, y, z )) << "  :  " << chunkManager.getBlockTypeInt(glm::vec3(new_x, y, z )) << std::endl;
					//std::cout << "Block for x: " << chunkManager.getBlockTypeInt(new_x,y,z)<< std::endl;
					if (chunkManager.getBlockTypeInt(glm::vec3(new_x, y, z ))){
						collided = true;
						std::cout << "Collision  X: " << new_x << " " <<  y << " " <<  z << " " << std::endl;
					}
				}
			}
			if (!collided){
				correct_motion.x = motion.x;
				pos.x += motion.x;
			}
		}

		if(motion.y < 0){
			new_x = floorf(pos.y + motion.y);
			testY = true;
		}
		else if(motion.y > 0){
			new_y = floorf(pos.y + motion.y + PLAYER_HEIGHT);
			testY = true;
		}

		

		if(testY){
			collided = false;
			for (int x=xMin; x<=yMax; x++){
				for (int z=zMin; z<=zMax; z++){
					std::cout << "Block at Pos "<< vec3_toString(new_position) <<" : " << chunkManager.getBlockTypeInt(new_position)<< std::endl;
					std::cout << "pos for  Y: " << vec3_toString(glm::vec3(x, new_y, z )) << "  :  " << chunkManager.getBlockTypeInt(glm::vec3(x, new_y, z )) << std::endl;
					if (chunkManager.getBlockTypeInt(glm::vec3(x,new_y,z))){
						collided = true;
						std::cout << "Collision  Y: " << x << " " <<  new_y << " " <<  z << " " << std::endl;
					}
				}
			}
			if (!collided){
				correct_motion.y = motion.y;
				pos.x += motion.y;
			}
		}

		if(motion.z < 0){
			new_z = floorf(pos.z + motion.z - PLAYER_RADIUS);
			testZ = true;
		}
		else if(motion.z > 0){
			new_z = floorf(pos.z + motion.z + PLAYER_RADIUS);
			testZ = true;
		}

		if (testZ){
			collided = false;
			for (int x=xMin; x<=yMax; x++){
				for (int y=yMin; y<=yMax; y++){
					if (chunkManager.getBlockTypeInt(glm::vec3(x,y,new_z))){
						collided = true;
						std::cout << "Collision  Z: " << x << " " <<  y << " " <<  new_z << " " << std::endl;
					}
				}
			}
			if (!collided){
				correct_motion.z = motion.z;
				pos.x += motion.z;
			}
		} */




		//camera.translate(correct_motion);
		camera.translate(new_position - position);
		update();
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
		//std::cout << "Total number of Chunks: " << counter << std::endl;
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
				//std::cout << "\n stepPos: " << vec3_toString(stepPos) << std::endl;
				chunkManager.setBlock(stepPos, 0);
				found = true;
				break;
			}
		}
		//printf("--------------------------------%p\n", stepPos);
/* 		if (!found){
			std::cout << "\n-not found  stepPos: " << vec3_toString(stepPos) << std::endl;
			std::cout << "\n-not found  stepPos: " << vec3_toString(position_in_chunk(stepPos)) << std::endl;
		} */
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
				if (glm::length(viewpos - previousPos) > 1.4) {
					//std::cout << "\npos: " << vec3_toString(position)
					//	      << "\npreviousPos: " << vec3_toString(previousPos) <<  std::endl;
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
	float speed = 24.0f;
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



