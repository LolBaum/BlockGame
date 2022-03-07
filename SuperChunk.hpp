#pragma once

#include <vector>

#include "printFunktions.hpp"
#include "util_funcs.hpp"

#include "Chunk.hpp"
#include "Block.hpp"
#include "local_player_data.hpp"
#include "textures.hpp"


#include "glm/glm.hpp"
#include <glm/gtc/noise.hpp> //swap with own header

class SuperChunk
{
public:
	
	SuperChunk(){

	}
	~SuperChunk() {}

	void initialize(){
		shader.initialize("shaders/basic.vs", "shaders/basic.fs");
		colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
		if (colorUniformLocation != -1) {
			GLCALL(glUniform4f(colorUniformLocation, 1.0f, 0.0f, 1.0f, 1.0f));
		}
		TextureUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_texture");
		if (TextureUniformLocation != -1) {
			GLCALL(glUniform1i(TextureUniformLocation, 0));
		}
		modelViewProjMatrixLocation = GLCALL(glGetUniformLocation(shader.getShaderId(), "u_modelViewProj"));
		tile_atlas.load("Graphics/TileAtlas64.png");
	}

	void addChunk(int x, int y, int z) {
		Chunk* newChunk = new Chunk(glm::vec3(x, y, z));
		chunks.push_back(newChunk);
		generateChunk(newChunk);
		std::cout << "added Chunk to SuperChunk at " << x << ", " << y << ", " << z << ", " << std::endl;
	}

	void loadChunk_new(glm::vec3 pos) {
		if (pos.y <= 128 && pos.y >= 0) {
			bool found_empty_chunk = false;
			int i = 0;
			while (i < chunks.size() && !found_empty_chunk) {
				//std::cout << "CHunk Pos:  " << vec3_toString(chunks.at(i)->getPos()) << std::endl;
				if (isChunkEmty(chunks.at(i)->getPos())) {
					found_empty_chunk = true;
					chunks.at(i)->setPos(pos);
					generateChunk(chunks.at(i));
					//std::cout << "CHunk Pos FOUND :  " << vec3_toString(chunks.at(i)->getPos()) << std::endl;
				}
				i++;
			}
			//std::cout << "found Empty Chunk:  " << found_empty_chunk << std::endl;
			if (!found_empty_chunk) {
				Chunk* newChunk = new Chunk(pos);
				chunks.push_back(newChunk);
				generateChunk(newChunk);
			}
		}

	}

	void loadChunk(int x, int y, int z) {
		loadChunk_new(glm::vec3(x, y, z));
		//Chunk* newChunk = new Chunk(glm::vec3(x, y, z));
		//chunks.push_back(newChunk);
		//dummy_genChunk(newChunk);
		//std::cout << "ADDED Chunk to SuperChunk at " << x << ", " << y << ", " << z << ", " << std::endl;
	}
	void loadChunk(glm::vec3 pos) {
		loadChunk_new(glm::vec3(pos));
		//Chunk* newChunk = new Chunk(pos);
		//chunks.push_back(newChunk);
		//dummy_genChunk(newChunk);
		//std::cout << "ADDED Chunk to SuperChunk at " << x << ", " << y << ", " << z << ", " << std::endl;
	}

	void unloadChunk_new(glm::vec3 pos) {
		Chunk* chunk = getChunk(pos);
		if (chunk != NULL) {
			chunk->clearChunk();
		}
		
	}

	void unloadChunk(int x, int y, int z) {
		unloadChunk_new(glm::vec3(x, y, z));
		//int index = this->getChunk_index(glm::vec3(x, y, z));
		//if (index >= 0) {
		//	chunks.at(index)->~Chunk();
		//	chunks.erase(chunks.begin() + index);
		//	//std::cout << "REMOVED Chunk to SuperChunk at " << x << ", " << y << ", " << z << ", " << std::endl;
		//}
	}
	void unloadChunk(glm::vec3 pos) {
		unloadChunk_new(pos);
		//int index = this->getChunk_index(pos);
		//if (index >= 0) {
		//	//chunks.at(index)->~Chunk();
		//	//chunks.erase(chunks.begin() + index);
		//	chunks.at(index)->clearChunk();
		//	//std::cout << "REMOVED Chunk to SuperChunk at " << x << ", " << y << ", " << z << ", " << std::endl;
		//}

	}


	//void addChunk_2(int x, int y, int z) {
	//	Chunk* newChunk = new Chunk(glm::vec3(x, y, z));
	//	chunks.push_back(newChunk);
	//	newChunk->dummyWorldGen(2);
	//	std::cout << "added Chunk to SuperChunk at " << x << ", " << y << ", " << z << ", " << std::endl;
	//}

	//void dummy_genChunk(Chunk* chunk) {
	//	chunk->dummyWorldGen(5);
	//}

	void generateChunk(Chunk* chunk){
		//std::cout << "Generating Chunk " << vec3_toString(chunk->getPos()) << std::endl;
		float pos_x = chunk->getPos().x;
		float pos_z = chunk->getPos().z;
		int pos_y = chunk->getPos().y;
		int max_y = pos_y + CY;
		float frequency_a = 16;
		float frequency_b = 128;
		int gras_hight = 2;
		//std::cout << "Chunk: " << vec3_toString(chunk->getPos()) << std::endl;
		for (int x = 0; x < CX; x++) {
			for (int z = 0; z < CY; z++) {
				float a = (glm::perlin(glm::vec2((x + pos_x) / frequency_a, (z + pos_z) / frequency_a)) + 1) / 2;
				float b = (glm::perlin(glm::vec2((x + pos_x) / frequency_b, (z + pos_z) / frequency_b)) + 1) / 2 * 48;// + (glm::perlin(glm::vec2((x + pos_x) / 1000, (z + pos_z) / 1000)) + 1)*20;
				int h = a * b;

				//int h = (glm::abs(glm::perlin(glm::vec2((x + pos_x) / 16.0, (z + pos_z) / 16.0)))) * 16;
				//std::cout << "perlin: " << h << std::endl;
				if (h >= pos_y) {
					for (int y = 0; (y + pos_y <= h-gras_hight) && (y + pos_y <max_y); y++) {
						chunk->setBlock(x, y, z, 1);
						//blocks[x][y][z].setId(1);
						//if (y + pos_y == h) {
						//	chunk->setBlock(x, h - pos_y, z, 2);
						//	
						//	std::cout << "h: " << h << std::endl << std::endl;
						//}
					}
					if (h<=max_y)
					{
						for (int i = h - gras_hight; i <= h; i++) {
							chunk->setBlock(x, i - pos_y, z, 2);// add: check if boundings are correct
						}
						 
					}
					//else
					//{
					//	std::cout << "max_y: " << max_y << std::endl;
					//	std::cout << "h: " << h << std::endl << std::endl;
					//}
				}

			}
		}
		chunk->updateMesh();
	}

	void render(const GLfloat* modelViewProj) {
		shader.bind();
		for (int i = 0; i < chunks.size(); i++) {
			chunks.at(i)->render(modelViewProjMatrixLocation, modelViewProj, tile_atlas.get_textureId());
			//std::cout << "renderd Chunk NR " << i << " at " << chunks.at(i)->getPos().x << ", " << chunks.at(i)->getPos().y << ", " << chunks.at(i)->getPos().z << ", " << std::endl;
		}
		shader.unbind();
	}

	Chunk* getChunk(glm::vec3 pos) {
		Chunk* chunk = nullptr;
		for (int i=0; i < chunks.size(); i++) {
			if (compareVec3(chunks.at(i)->getPos(), pos)) {
				return chunks.at(i);
			}
		}
		return nullptr; // kann evtl. verbessert werden...
	}


	Chunk* getChunk(int x, int y, int z) {
		Chunk* chunk = nullptr;
		glm::vec3 pos = glm::vec3(x, y, z);
		for (int i = 0; i < chunks.size(); i++) {
			if (compareVec3(chunks.at(i)->getPos(), pos)) {
				return chunks.at(i);
			}
		}
		return nullptr; // kann evtl. verbessert werden...
	}


	int getChunk_index(glm::vec3 pos) {
		Chunk* chunk = nullptr;
		for (int i = 0; i < chunks.size(); i++) {
			if (compareVec3(chunks.at(i)->getPos(), pos)) {
				return i;
			}
		}
		return -1;
	}

	void loadChunks(std::vector<glm::vec3> chunkIds) {
		bool unload = true;
		std::cout << "--- Loading Chunks ---" << std::endl;
		//clearChunkLoadingData();
		int i = 0;
		while (i < chunks.size()) {
			int j = 0;
			unload = true;
			if (!isChunkEmty(chunks.at(i)->getPos())) {
				while (j < chunkIds.size()) {
					if (chunks.at(i)->getPos() == chunkIds.at(j)) {
						//std::cout << "found Chunk " << vec3_toString(chunkIds.at(j)) << std::endl;
						unload = false;
						chunkIds.erase(chunkIds.begin() + j);
						
						j--;
					}
					j++;
				}
				//std::cout << "Chunk " << vec3_toString(chunks.at(i)->getPos()) << ", unload: " << unload << std::endl;
				if (unload) {
					//chunks_to_unload.push_back(chunks.at(i)->getPos());
					unloadChunk(chunks.at(i)->getPos());
					//std::cout << "Unloaded Chunk " << vec3_toString(chunks.at(i)->getPos()) << std::endl;
				}
			}
			i++;
		}
		for (int i = 0; i < chunkIds.size(); i++){
			loadChunk(chunkIds.at(i));
			//chunks_to_load.push_back(chunkIds.at(i));
		}
		//std::cout << posVector_toString(chunks_to_load, "Chunks to load") << std::endl;
		//std::cout << posVector_toString(chunks_to_unload, "Chunks to unload") << std::endl;
	}
	void clearChunkLoadingData() {
		chunks_to_load.clear();
		chunks_to_unload.clear();
	}
	void updateChunkLoadingData(std::vector<glm::vec3> chunkIds) {
		clearChunkLoadingData();
		bool unload = true;

		for (int i = 0; i < chunks.size(); i++) {
			unload = true;
			int j = 0;
			if (!isChunkEmty(chunks.at(i)->getPos())) {
				while (j < chunkIds.size()) {
					if (chunks.at(i)->getPos() == chunkIds.at(j)) {
						unload = false;
						chunkIds.erase(chunkIds.begin() + j);
						j--;
					}
					j++;
				}
				if (unload) {
					chunks_to_unload.push_back(chunks.at(i)->getPos());
					//unloadChunk(chunks.at(i)->getPos());
				}
			}
		}
		for (int i = 0; i < chunkIds.size(); i++) {
			if(! isChunkEmty(chunkIds.at(i))){
				chunks_to_load.push_back(chunkIds.at(i));
			}
		}


		//std::cout << posVector_toString(chunks_to_load, "Chunks to load") << std::endl;
		//std::cout << posVector_toString(chunks_to_unload, "Chunks to unload") << std::endl;
	}

	void load_unload_singleChunk() {
		if (chunks_to_unload.size() > 0) {
			unloadChunk(chunks_to_unload.at(0));
			chunks_to_unload.erase(chunks_to_unload.begin());
		}
		if (chunks_to_load.size() > 0){
			loadChunk(chunks_to_load.at(0));
			chunks_to_load.erase(chunks_to_load.begin());

		}

	}


	int getNumChunks() {
		return chunks.size();
	}
	int getNumFilledChunks() {
		int n = 0;
		for (int i=0; i<chunks.size(); i++){
			if (!chunks.at(i)->is_empty()){
				n++;
			}
		}
		return n;
	}


	std::vector<Chunk*>getChunks() {
		return  chunks;
	}

	int getBlockTypeInt(int x, int y, int z) {
		int cx = floor(x / CX) * CX;
		int cy = floor(y / CY) * CY;
		int cz = floor(z / CZ) * CZ;


		Chunk* chunk = getChunk(cx, cy, cz);
		if (!chunk) {
			return 0;
		}
		else {
			return chunk->getBlockTypeInt(position_in_chunk(glm::vec3(x,y,z)));
		}
	}
	int getBlockTypeInt(glm::vec3 pos) {
		int cx = floor(pos.x / CX) * CX;
		int cy = floor(pos.y / CY) * CY;
		int cz = floor(pos.z / CZ) * CZ;


		Chunk* chunk = getChunk(cx, cy, cz);
		if (!chunk) {
			return 0;
		}
		else {
			return chunk->getBlockTypeInt(position_in_chunk(pos));
		}
	}


	void setBlock(glm::vec3 pos, int type=0) {
		int cx = floor(pos.x / CX) * CX;
		int cy = floor(pos.y / CY) * CY;
		int cz = floor(pos.z / CZ) * CZ;

		std::cout << "cx: " << cx << ", cy: " << cy << ", cz: " << cz << std::endl;

		Chunk* chunk = getChunk(cx, cy, cz);
		if (!chunk) {
			return;
		}
		else {
			std::cout << "pos in chunk: " <<vec3_toString(position_in_chunk(pos)) << std::endl;
			chunk->setBlock(position_in_chunk(pos), type);
		}
	}

	Shader get_shader(){
		return shader;
	} 
	Texture get_tile_atlas(){
		return tile_atlas;
	} 
private:
	std::vector<Chunk*> chunks;
	std::vector<glm::vec3> chunks_to_load;
	std::vector<glm::vec3> chunks_to_unload;

	int colorUniformLocation;
	int TextureUniformLocation;
	int modelViewProjMatrixLocation;
	Shader shader = Shader();
	Texture tile_atlas;
};


SuperChunk chunkManager;// = SuperChunk();

