#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <map>
// replace with a custom string class
#include <string>







class Config
{
private:
    /* data */
    static std::map<std::string, int> intData;
    static std::map<std::string, float> floatData;
    static std::map<std::string, std::string> stringData;
    static std::map<std::string, bool> boolData;

    Config(/* args */);
    static void locateConfigFile();
    static void loadConfigFile();
    static void writeConfigFile();

public:

    static int getIntValue(std::string);
    static float getFloatValue(std::string);
    static std::string getStringValue(std::string);
    static bool getBoolValue(std::string);

    static void setIntValue(std::string, int);
    static void setFloatValue(std::string, float);
    static void setStringValue(std::string, std::string);
    static void setBoolValue(std::string, bool);

    static void init();
    static void saveConfig();
   
};








#endif