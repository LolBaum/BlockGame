//
// Created by Baum on 28.01.2025.
//

#include "WorldGenerator.hpp"
#include "SuperChunk.hpp"
#include <condition_variable>

WorldGenerator::WorldGenerator(moodycamel::ReaderWriterQueue<shared_ptr<Chunk*>>* in,
                               moodycamel::ReaderWriterQueue<shared_ptr<Chunk*>>* out) {
    ChunkGenQueueIn = in;
    ChunkGenQueueOut = out;
    threadShouldRun = true;
    thread = std::thread(&WorldGenerator::runWorldGenThread, this);
}

WorldGenerator::~WorldGenerator() {
    threadShouldRun = false;
    thread.join();
}

void WorldGenerator::generateChunk(Chunk *chunk) {
    std::cout << "Generating Chunk " << vec3_toString(chunk->getPos()) << std::endl;
    float pos_x = chunk->getPos().x;
    float pos_z = chunk->getPos().z;
    int pos_y = chunk->getPos().y;
    int max_y = pos_y + CY;
    float frequency_a = 16;
    float frequency_b = 128;
    int gras_hight = 2;
    //std::cout << "Chunk: " << vec3_toString(chunk->getPos()) << std::endl;
//    for (int x = 0; x < CX; x++) {
//        for (int z = 0; z < CY; z++) {
//            float a = (glm::perlin(glm::vec2((x + pos_x) / frequency_a, (z + pos_z) / frequency_a)) + 1) / 2;
//            float b = (glm::perlin(glm::vec2((x + pos_x) / frequency_b, (z + pos_z) / frequency_b)) + 1) / 2 * 48;// + (glm::perlin(glm::vec2((x + pos_x) / 1000, (z + pos_z) / 1000)) + 1)*20;
//            //int h = a * b;
//            int h = a * b/4 + 20;
//
//            if (h >= pos_y) {
//                for (int y = 0; (y + pos_y <= h-gras_hight) && (y + pos_y <max_y); y++) {
//                    chunk->setBlock(x, y, z, 1);
//                }
//                if (h<=max_y)
//                {
//                    for (int i = h - gras_hight; i <= h-1; i++) {
//                        chunk->setBlock(x, i - pos_y, z, 2);// add: check if boundings are correct
//                    }
//                    chunk->setBlock(x, h - pos_y, z, 3);
//
//                    float grass_val = (glm::perlin(glm::vec2((x + pos_x) /4, (z + pos_z) / 4)));
//                    if (grass_val >0.5){
//                        //chunk->setBlock(x, h - pos_y+1, z, 4);
//                        SuperChunk::setBlock((int)(x + pos_x), h+1, (int)(z + pos_z), 4);
//                    }
//                }
//            }
//        }
//    }

    float max_h = 128;
    float fac = 64;
    for (int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            float h_fac = (y + pos_y) / max_h;
            for (int z = 0; z < CY; z++) {
                float noise_val = glm::abs(
                        glm::perlin(glm::vec3((x + pos_x) / fac, (y + pos_y) / fac, (z + pos_z) / fac)));
                //std::cout << "perlin: " << h << std::endl;
                float val = ((noise_val - 1) + (h_fac*2)) * 64;
                if (val < 0.5) {
                    chunk->setBlock(x,y,z, 1);
                }
            }
        }

    }
}

void WorldGenerator::runWorldGenThread() {
    std::condition_variable cv;
    shared_ptr<Chunk*> chunk;
    while (threadShouldRun){
        //TODO: make thread wait
        while (ChunkGenQueueIn->try_dequeue(chunk) && threadShouldRun){
            generateChunk(*chunk.get());
            ChunkGenQueueOut->enqueue(chunk);
        }
    }
}
