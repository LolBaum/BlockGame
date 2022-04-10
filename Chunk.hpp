#pragma once
#include <glm/ext.hpp>
#include <glm/gtc/noise.hpp>

//#include "vertex_buffer.hpp"
//#include "index_buffer.hpp"
#include "Block.hpp"
#include "object_models.hpp"


// forward declarations

/* class BlockType;
struct Block; */



struct Chunk{
	Chunk(glm::vec3 position = glm::vec3(0,0,0));
	~Chunk();

	void clearChunk();

	void setBlock(int x, int y, int z, int type);
	void setBlock(glm::vec3 pos, int type);

	int getBlockTypeInt(int x, int y, int z);
	int getBlockTypeInt(glm::vec3 pos);

	BlockType* getBlockType(int x, int y, int z);

	Block* getBlock(int x, int y, int z);

	void dummyWorldGen(int variant=0);

	void updateMesh();

	void render(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId);

	void render_transparent(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId);

	void calculateNumFaces();

	int getNumMeshIndices();
	int getMaxNumfaces();
	int getNumfaces();
	ChunkMesh* getMesh();


	glm::vec3 getPos();
	void setPos(glm::vec3 newPos);
	unsigned int get_numBlocks();
	bool is_empty();

private:
	Block blocks[CX][CY][CZ];
	glm::vec3 pos;
	unsigned int numBlocks;
	bool changed;

	int num_faces;
	ChunkMesh mesh;

	int num_faces_transparent;
	ChunkMesh mesh_transparent;


}; 
//Chunk NullChunk = *new Chunk(glm::vec3(0,0,0));
