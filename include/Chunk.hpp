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

    const unsigned long long getId();

	void clearChunk();

	void setBlock(int x, int y, int z, int type, BlockRotation rot=Y_positive);
	void setBlock(glm::vec3 pos, int type, BlockRotation rot=Y_positive);

	int getBlockTypeInt(int x, int y, int z);
	int getBlockTypeInt(glm::vec3 pos);

	BlockType* getBlockType(int x, int y, int z);

	Block* getBlock(int x, int y, int z);

	void dummyWorldGen(int variant=0);

	void updateMesh();

	void render(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId);

    void render_foliage(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId);

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
    void getIDPos(int *x, int *y, int *z);


	////////////////////////////
	// Chunk Loading //////////
	//////////////////////////

	void serialize(const std::string& worldSavePath);
	bool deserialize(const std::string& worldSavePath, int load_x, int load_y, int load_z);



private:
	Block blocks[CX][CY][CZ];
	glm::vec3 pos;
    unsigned long long id;
	unsigned int numBlocks;
	bool changed;
	bool changed_since_loading = false;

	int num_faces;
	ChunkMesh mesh;

    int num_faces_foliage;
    ChunkMesh mesh_foliage;

	int num_faces_transparent;
	ChunkMesh mesh_transparent;

    int px;
    int py;
    int pz;


};
//Chunk NullChunk = *new Chunk(glm::vec3(0,0,0));
