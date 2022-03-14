#pragma once
#include <glm/ext.hpp>
#include <glm/gtc/noise.hpp>

#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "object_models.hpp"
#include "Block.hpp"
#include "util_funcs.hpp"
#include "printFunktions.hpp"




struct Chunk{
	Chunk(glm::vec3 position = glm::vec3(0,0,0)) {
		pos = position;
		numBlocks = 0;
		changed = true;

		vertexBuffer = *new ChunkVertexBuffer();
		indexBuffer = *new IndexBuffer();
	}

	~Chunk(){
		//std::cout << "Called Destructor for Chunk: " << pos.x << ", " << pos.y << ", " << pos.z << ", " << std::endl;
		vertexBuffer.~ChunkVertexBuffer();
		indexBuffer.~IndexBuffer();
		mesh.~ChunkMesh();
	}

	void clearChunk() {
		//std::cout << "cleared Chunk: " << vec3_toString(pos) << std::endl;
		pos = glm::vec3(-1, -1, -1);
		for (int x = 0; x < CX; x++) {
			for (int y = 0; y < CY; y++) {
				for (int z = 0; z < CZ; z++) {
					blocks[x][y][z].setId(0);
				}
			}
		}
		changed = true;
		updateMesh();
		
	}

	void setBlock(int x, int y, int z, int type) {
		if (0 <= x && x <= 15 && 0 <= y && y <= 15 && 0 <= z && z <= 15) {
			blocks[x][y][z].setId(type);
			changed = true;
		}
	} //add changes
	void setBlock(glm::vec3 pos, int type) {
		int x = pos.x;
		int y = pos.y;
		int z = pos.z;

		if (0 <= x && x <= 15 && 0 <= y && y <= 15 && 0 <= z && z <= 15) {
			blocks[x][y][z].setId(type);
			changed = true;
			std::cout << "set Block at: " << vec3_toString(pos) << std::endl;
			return;
		}
	}

	int getBlockTypeInt(int x, int y, int z) {
		if (0 <= x && x <= 15 && 0 <= y && y <= 15 && 0 <= z && z <= 15) {
			return blocks[x][y][z].getId();
		}
		else {
			return 0;
		}
	}
	int getBlockTypeInt(glm::vec3 pos) {
		int x = pos.x;
		int y = pos.y;
		int z = pos.z;
		if (0 <= x && x <= 15 && 0 <= y && y <= 15 && 0 <= z && z <= 15) {
			return blocks[x][y][z].getId();
		}
		else {
			return 0;
		}
	}

	BlockType* getBlockType(int x, int y, int z) {
		if (0 <= x && x <= 15 && 0 <= y && y <= 15 && 0 <= z && z <= 15) {
			return btm.GetBlockType(blocks[x][y][z].getId());
		}
		else {
			return &btm.Air;
		}
	}

	Block* getBlock(int x, int y, int z) {
		if (0 <= x <= 15 && 0 <= y <= 15 && 0 <= z <= 15) {
			return &blocks[x][y][z];
		}
		return 0; // be carefull, check for NULL when using this function
	}

	void dummyWorldGen(int variant=0) {
		//std::cout << "filled Chunk: " << vec3_toString(pos) << std::endl;
		switch (variant)
		{
		case 0:
			for (int x = 0; x < CX; x++) {
				for (int y = 0; y < x; y++) {
					for (int z = 0; z < CZ; z++) {
						blocks[x][y][z].setId(1);
					}
				}
			}
			break;
		case 1:
			for (int x = 0; x < CX; x++) {
				for (int y = 0; y < 1; y++) {
					for (int z = 0; z < CZ; z++) {
						blocks[x][y][z].setId(1);
					}}}
			break;
		case 2:
			for (int x = 0; x < 2; x++) {
				for (int y = 0; y < CY; y++) {
					for (int z = 0; z < CZ; z++) {
						blocks[x][y][z].setId(1);
					}
				}
			}
			break;
		case 3:
			for (int x = 0; x < CX; x++) {
				for (int y = 0; y < CY; y++) {
					for (int z = 0; z < CZ; z++) {
						if (((float)(x * x)/256.0 + (float)(y * y) /256.0 + (float)(z * z) / 256.0) < 1 &&
							((float)(x * x) / 256.0 + (float)(y * y) / 256.0 + (float)(z * z) / 256.0) > 0.5) {
							//std::cout << ((float)(x * x) / 256.0 + (float)(y * y) / 256.0) << std::endl;
							blocks[x][y][z].setId(1);
						}
					}
				}
			}
			break;
		case 4:
			for (int x = 0; x < CX; x++) {
				for (int y = 0; y < CY; y++) {
					for (int z = 0; z < CZ; z++) {
						float x1 = ((x - 8) * (x - 8)) / 128.0;
						float y1 = ((y - 8) * (y - 8)) / 128.0;
						float z1 = ((z - 8) * (z - 8)) / 128.0;
						
						if ((x1+y1+z1) < 0.3) {
							
							blocks[x][y][z].setId(1);
						}
					}
				}
			}
			break;
		case 5:
			for (int x = 0; x < CX; x++) {
				for (int z = 0; z < CY; z++) {
					int h = (glm::abs(glm::perlin(glm::vec2((x+pos.x)/16.0, (z+pos.z)/16.0))))*16;
					//std::cout << "perlin: " << h << std::endl;
					for (int y = 0; y <= h; y++) {
						blocks[x][y][z].setId(1);
					}
					
					
				}
			}
			break;
		case 6:
			for (int x = 0; x < CX; x++) {
				for (int y = 0; y < CY; y++) {
					for (int z = 0; z < CY; z++) {
						float h = glm::abs(glm::perlin(glm::vec3((x + pos.x) / 16.0, (y + pos.y) / 16.0, (z + pos.z) / 16.0)));
						//std::cout << "perlin: " << h << std::endl;
						if (h < 0.2) {
							blocks[x][y][z].setId(1);
						}
					}
				}

			}
			break;
		default:
			break;
		}
		changed = true;
		updateMesh();
	}

	void updateMesh() {
		//std::cout << "Updating the mesh" << std::endl;
		
		if(changed){

			if (! isChunkEmty(pos)) {
				//std::cout << "Updating mesh: Pos: " << vec3_toString(pos) << std::endl;
			}
			


			num_faces = calculateNumFaces();
			mesh.clearMesh();
			mesh.reserveFaces(num_faces);
			//std::cout << "mesh.reserveFaces("<<NumFaces<<");" << std::endl;
			numBlocks = 0;
			for (int x = 0; x < CX; x++) {
				for (int y = 0; y < CY; y++) {
					for (int z = 0; z < CZ; z++) {
						int id = blocks[x][y][z].getId();
						if (id != 0) {
							numBlocks++;
							//std::cout << glm::vec3((float32)x, y, z).x << " " <<glm::vec3(x, (float32)y, z).y << std::endl;
							//std::cout << "adding Block" << std::endl;

							glm::vec3 dummyPos = glm::vec3(pos.x, pos.y, pos.z);

							BlockType* bt = getBlockType(x,y,z);
							if (bt->isOpaque()){
							switch (bt->get_texture_type()) // ToDo: Get teh texture Type from the Blockmanager 
							{
								// Single Texture
							case SingleTexture: {
								int tex_x = 0;
								int tex_y = 0;
								bt->get_tex_coords(&tex_x, &tex_y);
								if (!getBlockType(x - 1, y, z)->isOpaque()) { // Todo: check for opeaqeness
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 3, tex_x, tex_y);
								}
								if (!getBlockType(x + 1, y, z)->isOpaque()) { // Todo: check for opeaqeness
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 1, tex_x, tex_y);
								}
								if (!getBlockType(x, y - 1, z)->isOpaque()) { // Todo: check for opeaqeness
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 4, tex_x, tex_y);
								}
								if (!getBlockType(x, y + 1, z)->isOpaque()) { // Todo: check for opeaqeness
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 5, tex_x, tex_y);
								}
								if (!getBlockType(x, y, z - 1)->isOpaque()) { // Todo: check for opeaqeness
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 2, tex_x, tex_y);
								}
								if (!getBlockType(x, y, z + 1)->isOpaque()) { // Todo: check for opeaqeness
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 0, tex_x, tex_y);
								}
								break;
							}
								  // Sides, Top, Bottom

								  // dummy test for Gras
							case MultiTexture: {
								//std::cout << "Mulittexture" << std::endl;
								int tex_x = 0;
								int tex_y = 0;
								SpecialBlockTexture* tex = bt->get_multi_texture();
								//std::cout << tex << std::endl;
								if (!getBlockType(x - 1, y, z)->isOpaque()) { // Todo: check for opeaqeness
									tex->get_left(&tex_x, &tex_y);
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 3, tex_x, tex_y); // left
								}
								 if (!getBlockType(x + 1, y, z)->isOpaque()) { // Todo: check for opeaqeness
									tex->get_right(&tex_x, &tex_y);
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 1, tex_x, tex_y); // right
								}
								if (!getBlockType(x, y - 1, z)->isOpaque()) { // Todo: check for opeaqeness
									tex->get_bottom(&tex_x, &tex_y);
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 4, tex_x, tex_y); // bottom
								}
								if (!getBlockType(x, y + 1, z)->isOpaque()) { // Todo: check for opeaqeness
									tex->get_top(&tex_x, &tex_y);
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 5, tex_x, tex_y); // top
								}
								if (!getBlockType(x, y, z - 1)->isOpaque()) { // Todo: check for opeaqeness
									tex->get_back(&tex_x, &tex_y);
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 2, tex_x, tex_y); // back
								}
								if (!getBlockType(x, y, z + 1)->isOpaque()) { // Todo: check for opeaqeness
									tex->get_front(&tex_x, &tex_y);
									mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 0, tex_x, tex_y); // front
								} 
								break;
							}
							default:
								break;
							}

							}
							else{
								int tex_x = 0;
								int tex_y = 0;
								bt->get_tex_coords(&tex_x, &tex_y);
								mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 3, tex_x, tex_y);
								mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 1, tex_x, tex_y);
								mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 4, tex_x, tex_y);
								mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 5, tex_x, tex_y);
								mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 2, tex_x, tex_y);
								mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 0, tex_x, tex_y);
							}

						}
					}
				}
			}
			//std::cout << "Chunk consits of " << numBlocks << " Blocks" << std::endl;

			uint32 numVertices = mesh.getNumVertices();
			Vertex* vertices = mesh.getVertices();

			uint32 numIndices = mesh.getNumIndices();
			uint32* indices = mesh.getIndices();

			indexBuffer.update(indices, numIndices, sizeof(indices[0])); // fix sizeof
			vertexBuffer.update(&vertices[0], numVertices);

			changed = false;
		}
		
	}

	void render(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId) {
		updateMesh();
		vertexBuffer.bind();
		indexBuffer.bind();

		GLCALL(glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, modelViewProj));
		GLCALL(glActiveTexture(GL_TEXTURE0));
		GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
		GLCALL(glDrawElements(GL_TRIANGLES, mesh.getNumIndices(), GL_UNSIGNED_INT, 0));

		indexBuffer.unbind();
		vertexBuffer.unbind();
	}

	int calculateNumFaces() {
		int numFaces = 0;
		int c = 0;
		for (int x = 0; x < CX; x++) {
			for (int y = 0; y < CY; y++) {
				for (int z = 0; z < CZ; z++) {
					if (blocks[x][y][z].getId() != 0) {
						c++;
						if (getBlockTypeInt(x - 1, y, z) == 0) { // Todo: check for opeaqeness
							numFaces++;
							//std::cout << "x - 1 "<<  std::endl;
						}
						if (getBlockTypeInt(x + 1, y, z) == 0) { // Todo: check for opeaqeness
							numFaces++;
							//std::cout << "x + 1" << std::endl;
						}
						if (getBlockTypeInt(x, y - 1, z) == 0) { // Todo: check for opeaqeness
							numFaces++;
							//std::cout << "y - 1" << std::endl;
						}
						if (getBlockTypeInt(x, y + 1, z) == 0) { // Todo: check for opeaqeness
							numFaces++;
							//std::cout << "y + 1" << std::endl;
						}
						if (getBlockTypeInt(x, y, z - 1) == 0) { // Todo: check for opeaqeness
							numFaces++;
							//std::cout << "z - 1" << std::endl;
						}
						if (getBlockTypeInt(x, y, z + 1) == 0) { // Todo: check for opeaqeness
							numFaces++;
							//std::cout << "z + 1" << std::endl;
						}
						
					}
				}
			}
		}
		return numFaces;
	}

	int getNumMeshIndices() { // remove this fuction?
		return mesh.getNumIndices();
	}
	int getMaxNumfaces() { // remove this fuction?
		return num_faces;
	}
	int getNumfaces() { // remove this fuction?
		return mesh.getNumVertices()/4;
	}

	ChunkMesh* getMesh() {
		return &mesh;
	}

	ChunkVertexBuffer* getVertexBuffer() {
		return &vertexBuffer;
	}
	IndexBuffer* getIndexBuffer() {
		return &indexBuffer;
	}

	void setVertexBuffer(ChunkVertexBuffer vb) {
		vertexBuffer = vb;
	}
	void setIndexBuffer(IndexBuffer ib) {
		indexBuffer = ib;
	}

	glm::vec3 getPos() {
		return pos;
	}
	void setPos(glm::vec3 newPos) {
		pos = newPos;
	}
	unsigned int get_numBlocks(){
		return numBlocks;
	}
	bool is_empty(){
		return (numBlocks==0);
	}

private:
	Block blocks[CX][CY][CZ];
	glm::vec3 pos;
	unsigned int numBlocks;
	bool changed;
	int num_faces;

	ChunkMesh mesh;
	ChunkVertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
}; 
//Chunk NullChunk = *new Chunk(glm::vec3(0,0,0));
