//
// Created by Baum on 28.01.2025.
//

#ifndef BLOCKGAME_WORLDGENERATOR_H
#define BLOCKGAME_WORLDGENERATOR_H


#include "readerwriterqueue.h"
#include "Chunk.hpp"
#include <thread>


class WorldGenerator {
public:
    WorldGenerator(moodycamel::ReaderWriterQueue<shared_ptr<Chunk*>>* ChunkGenQueueIn,
                   moodycamel::ReaderWriterQueue<shared_ptr<Chunk*>>* ChunkGenQueueOut);
    ~WorldGenerator();




private:
    moodycamel::ReaderWriterQueue<shared_ptr<Chunk*>>* ChunkGenQueueIn;
    moodycamel::ReaderWriterQueue<shared_ptr<Chunk*>>* ChunkGenQueueOut;
    void runWorldGenThread();

    void generateChunk(Chunk* chunk);


    std::thread thread;

    bool threadShouldRun;
};


#endif //BLOCKGAME_WORLDGENERATOR_H
