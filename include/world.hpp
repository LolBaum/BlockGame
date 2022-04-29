#pragma once
#ifndef WORLD_H
#define WORLD_H

#include <map>
// replace with a custom string class
#include <string>
#include "glm/glm.hpp"


#include "inventory.hpp"


// Currently basicly a copy of my Config class
// Will beused to store and load World Data
// e.g. seed, Worldgeneration Parameters

// Player Data should be put into another Class, but 
// That will be done later

// This class (andt the current chunk serialisation) could 
// eventually be replaced by using a database



class World
{
private:
    /* data */
    static std::map<std::string, int> intData;
    static std::map<std::string, float> floatData;
    static std::map<std::string, std::string> stringData;
    static std::map<std::string, bool> boolData;
    static std::map<std::string, glm::vec3> vec3Data;
    static std::map<std::string, Inventory> World::containerData;
    static std::string filePaht;

    World(/* args */);
    static void locateWorldFile();
    static void loadWorldFile();
    static void writeWorldFile();

public:

    static int getIntValue(std::string);
    static float getFloatValue(std::string);
    static std::string getStringValue(std::string);
    static bool getBoolValue(std::string);
    static glm::vec3 getvec3Value(std::string);

    static void setIntValue(std::string, int);
    static void setFloatValue(std::string, float);
    static void setStringValue(std::string, std::string);
    static void setBoolValue(std::string, bool);
    static void setvec3Value(std::string, glm::vec3);

    static void init();
    static void saveWorldFile();
   
};








#endif