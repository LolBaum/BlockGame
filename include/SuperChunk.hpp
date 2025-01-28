#pragma once

#include <vector>

#include "printFunktions.hpp"
#include "util_funcs.hpp"


#include "Block.hpp"
#include "Chunk.hpp"


#include "textures.hpp"
#include "shader.hpp"


#include "glm/glm.hpp"
#include "WorldGenerator.hpp"
#include <glm/gtc/noise.hpp> //swap with own header

#include <readerwriterqueue.h>

class SuperChunk
{
	SuperChunk();
	~SuperChunk();
public:
	
	
	static void initialize();

	static void addChunk(int x, int y, int z);

	static void loadChunk_new(glm::vec3 pos);

	static void loadChunk(int x, int y, int z);
	static void loadChunk(glm::vec3 pos);

	static void unloadChunk_new(glm::vec3 pos);

	static void unloadChunk(int x, int y, int z);
	static void unloadChunk(glm::vec3 pos);


	static void generateChunk(Chunk* chunk);

	static void render(const GLfloat* modelViewProj);

    static void render_foliage(const GLfloat* modelViewProj);

	static void render_transparent(const GLfloat* modelViewProj);

	static Chunk* getChunk(glm::vec3 pos);

	static Chunk* _getChunk(int x, int y, int z);

	static int getChunk_index(glm::vec3 pos);

	static void loadChunks(std::vector<glm::vec3> chunkIds);
	static void clearChunkLoadingData();
	static void updateChunkLoadingData(std::vector<glm::vec3> chunkIds);

	static void load_unload_singleChunk();

    static void getGeneratedChunks();


	static int getNumChunks();
	static int getNumFilledChunks();


	static std::vector<Chunk*>getChunks();

	static int getBlockTypeInt(int x, int y, int z);
	static int getBlockTypeInt(glm::vec3);
    static BlockType *getBlockType(int x, int y, int z);

	static bool has_Block_collision(glm::vec3 pos);


	static void setBlock(glm::vec3 pos, int type=0, BlockDirection rot=Y_positive);
    static void setBlock(int x, int y, int z, int type=0, BlockDirection rot=Y_positive);
	static int get_num_all_faces();

	static Shader & get_shader();
	static Texture* get_tile_atlas();

	static void saveWorld();
    static void renderDebug_Box(glm::vec3 p, GLfloat* modelViewProj);


private:
	static std::vector<Chunk*> chunks;
	static std::vector<glm::vec3> chunks_to_load;
	static std::vector<glm::vec3> chunks_to_unload;

	static int colorUniformLocation;
	static int TextureUniformLocation;
	static int modelViewProjMatrixLocation;
	static Shader shader;
	static Shader transparent_shader;
	static Texture tile_atlas;
    static Box* debugBox;

	static std::string worldSavePath;

    static moodycamel::ReaderWriterQueue<shared_ptr<Chunk*>> ChunkGenQueueIn;
    static moodycamel::ReaderWriterQueue<shared_ptr<Chunk*>> ChunkGenQueueOut;

    static WorldGenerator* worldGenerator;


};



