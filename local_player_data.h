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
	~LocalPlayer() {
	}

	void update() {
		//camera.translate(position);
		position = camera.getPos();
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
		//std::cout << "Updated the Local chunk IDs" << std::endl;
		chunksInSight.clear();

		//int dummy_max_x = currentChunkPos.x;
		//int dummy_max_y = currentChunkPos.y;
		//int dummy_max_z = currentChunkPos.z;

		//int dummy_min_x = currentChunkPos.x;
		//int dummy_min_y = currentChunkPos.y;
		//int dummy_min_z = currentChunkPos.z;

		for (int x = -sightDistance; x <= sightDistance; x++) {
			for (int y = -sightDistance; y <= sightDistance; y++) {
				for (int z = -sightDistance; z <= sightDistance; z++) {
					int pos_x = currentChunkPos.x + x * 16;
					int pos_y = currentChunkPos.y + y * 16;
					int pos_z = currentChunkPos.z + z * 16;
					//std::cout << "Adding Chunkt to local ID list" << vec3_toString(glm::vec3(pos_x, pos_y, pos_z)) << std::endl;
					chunksInSight.push_back(glm::vec3(pos_x, pos_y, pos_z));
					//if (pos_x > dummy_max_x) {
					//	dummy_max_x = pos_x;
					//}
					//if (pos_y > dummy_max_y) {
					//	dummy_max_y = pos_y;
					//}
					//if (pos_z > dummy_max_z) {
					//	dummy_max_z = pos_z;
					//}
					//if (pos_x < dummy_min_x) {
					//	dummy_min_x = pos_x;
					//}
					//if (pos_y < dummy_min_y) {
					//	dummy_min_y = pos_y;
					//}
					//if (pos_z < dummy_min_z) {
					//	dummy_min_z = pos_z;
					//}

				}
			}
		}
		//std::cout << "min x: " << dummy_min_x << std::endl;
		//std::cout << "min y: " << dummy_min_y << std::endl;
		//std::cout << "min z: " << dummy_min_z << std::endl;
		//std::cout << "max x: " << dummy_max_x << std::endl;
		//std::cout << "max y: " << dummy_max_y << std::endl;
		//std::cout << "max z: " << dummy_max_z << std::endl;
		//std::cout << vec3_toString(currentChunkPos, "Chunk Pos") << std::endl;
	}
	std::vector<glm::vec3> getLocalChunkIds() {
		return chunksInSight;
	}

	void get_focussed_Block() {
		glm::vec3 lookAt = camera.getLookAt();
		std::cout << vec3_toString(lookAt, "lookAt") << std::endl;
		glm::vec3 stepPos;
		bool found = false;
		for (int i = 0; i * APROX_STEP_SIZE < PLAYER_ACTION_RANGE; i++) {
			float factor = i * APROX_STEP_SIZE;
			stepPos = position + lookAt * factor; // multiplied Vector with scalar by using *. better change to function
			//std::cout << "stepPos: " << vec3_toString(stepPos) << std::endl;
			//std::cout << "factor: " << factor << std::endl;
			if (chunkManager.getBlockTypeInt(stepPos)) {
				std::cout << "\n stepPos: " << vec3_toString(stepPos) << std::endl;
				chunkManager.setBlock(stepPos, 0);
				found = true;
				break;
			}
		}
		printf("--------------------------------%p\n", stepPos);
		if (!found){
			std::cout << "\n-not found  stepPos: " << vec3_toString(stepPos) << std::endl;
			std::cout << "\n-not found  stepPos: " << vec3_toString(position_in_chunk(stepPos)) << std::endl;
		}
		//if (pos_x != 0) {
		//}
		//float fac_x = 1.0/lookAt.x;
		//std::cout << "X: " << std::endl;
		//for (int i = 0; i < PLAYER_ACTION_RANGE; i++) {
		//	dummyVec = lookAt * (i * fac_x) + currentChunkPos;
		//	pos_x = lookAt.x * (i * fac_x) + currentChunkPos.x;
		//	pos_y = lookAt.y * (i * fac_x) + currentChunkPos.y;
		//	pos_z = lookAt.z * (i * fac_x) + currentChunkPos.z;
		//	std::cout << vec3_toString(dummyVec, "dummyVec") << std::endl;
		//	std::cout << "pos_x: " << pos_x << ", pos_y: " << pos_y << ", pos_z: " << pos_z << std::endl;
		//	std::cout << "----- Block id: " << chunkManager.getBlockTypeInt(pos_x, pos_y, pos_z) << std::endl;
		//}
		//std::cout << "----- Block id at PlayerPos: " << chunkManager.getBlockTypeInt(position) << std::endl;
		//std::cout << "----- pos in chunk: " << vec3_toString(position_in_chunk(position)) << std::endl;
		
	}
	void place_block() {
		glm::vec3 lookAt = camera.getLookAt();
		std::cout << vec3_toString(lookAt, "lookAt") << std::endl;
		glm::vec3 stepPos;
		glm::vec3 previousPos = position;
		bool found = false;
		for (int i = 0; i * APROX_STEP_SIZE < PLAYER_ACTION_RANGE; i++) {
			float factor = i * APROX_STEP_SIZE;
			stepPos = position + lookAt * factor; // multiplied Vector with scalar by using *. better change to function
			if (chunkManager.getBlockTypeInt(stepPos)) {
				if (glm::length(position - previousPos) > 1.4) {
					std::cout << "\npos: " << vec3_toString(position)
						      << "\npreviousPos: " << vec3_toString(previousPos) <<  std::endl;
					chunkManager.setBlock(previousPos, 1);
					found = true;
				}
				
				break;
			}
			previousPos = stepPos;
			//
		}
	}

private:
	float speed = 24.0f;
	int sightDistance = 4;
	std::vector<glm::vec3> chunksInSight;
	glm::vec3 position;
	glm::vec3 currentChunkPos;
	glm::vec3 lastChunkPos;
	bool changedChunk = true;

	FloatingCamera camera = *new FloatingCamera();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 modelViewProj = camera.getViewProj() * model;
	

	//int dummy_max_x;
	//int dummy_max_y;
	//int dummy_max_z;

	//int dummy_min_x;
	//int dummy_min_y;
	//int dummy_min_z;
};



