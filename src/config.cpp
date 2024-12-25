#include "config.hpp"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


template<typename type>
void print_map(string comment, const map<string, type>& m){
    cout << comment ;
    for (const auto& n : m) {
        cout << n.first << " = " << n.second << "; ";
    }
    cout << '\n';
}

template<typename type>
void add_map_to_json(json& j, const map<string, type>& m){
    for (const auto& n : m) {
        j[n.first] = n.second;
    }
}

template<typename type>
void map_set(map<string, type>& m, string key, type value){
    typename map<string, type>::iterator it = m.find(key);
    if (it != m.end()){
        it->second = value;
    }
    else{
        m.insert(pair<string, type> (key, value));
    } 
}

map<string, int> Config::intData;
map<string, float> Config::floatData;
map<string, string> Config::stringData;
map<string, bool> Config::boolData;


Config::Config(){}

void Config::init(){
    intData.insert(pair<string,int> (string("WindowX"), 1440) );
    intData.insert(pair<string,int> (string("WindowY"), 900 ) );
    intData.insert(pair<string,int> (string("SightDistance"), 6));


    stringData.insert(pair<string, string> (string("ShaderPath"), string("shaders/")));
    stringData.insert(pair<string, string> (string("TileAtlasPathName"), string("graphics/TileAtlas64_Quandary_1_1.png")));
    stringData.insert(pair<string, string> (string("FontPathName"), string("graphics/fontAtlas.png")));
    stringData.insert(pair<string, string> (string("WorldSavePath"), string("world")));

    floatData.insert(pair<string,float> (string("Fov"), 91.5f));

    // "Fullscreen" will use Borderless Mode 
    boolData.insert(pair<string,bool> (string("Fullscreen"), false));
    boolData.insert(pair<string,bool> (string("WindowMaximezed"), false));
    //boolData.insert(pair<string,bool> (string("WindowBorderless"), false));
    boolData.insert(pair<string,bool> (string("WindowResizeable"), false));
    boolData.insert(pair<string,bool> (string("FancySky"), false));

    //print_map("[Config-info] int: ", intData);
    //print_map("[Config-info] string: ", stringData);
    //print_map("[Config-info] float: ", floatData);
    //print_map("[Config-info] bool: ", boolData);

    loadConfigFile();

    //print_map("[Config-info] int: ", intData);
    //print_map("[Config-info] string: ", stringData);

}

void Config::locateConfigFile(){}

void Config::loadConfigFile(){
    std::cout << "[INFO] Loading Config" << std::endl;
    string line;
    string file_name = "config.json";
    ifstream file (file_name);
    json j;
    if (file.is_open()){
        file >> j;
        //std::cout << j << endl;
        for (auto& el : j.items()){
            switch (el.value().type())
            {
            case json::value_t::number_integer: 
            case json::value_t::number_unsigned:
                map_set(intData, el.key(), (int)el.value());
                break;
            case json::value_t::number_float:
                map_set(floatData, el.key(), (float)el.value());
                break;
            case json::value_t::string:
                map_set(stringData, el.key(), (string)el.value());
                break;
            case json::value_t::boolean:
                map_set(boolData, el.key(), (bool)el.value());
                break;
            
            default:
                cout << "[Config] loading : " << el.key() << " value " << el.value() << " is type " <<  el.value().type_name() << " which is not implemented yet" << endl;
                break;
            }
            //std::cout << "key: " << el.key() << ", value:" << el.value() << ", type: " << el.value().type_name() << '\n';
        }
    }
    else {
        cout << "Unable to open config file: " << file_name << endl;
    } 
}

void Config::writeConfigFile(){

    string file_name = "config.json";
    ofstream file (file_name);

    json j;
    add_map_to_json(j, intData);
    add_map_to_json(j, stringData);
    add_map_to_json(j, floatData);
    add_map_to_json(j, boolData);


    if (file.is_open())
    {
        file << j.dump(4);
        file.close();
    }
    else cout << "Unable to open file " << file_name << endl;
}

int Config::getIntValue(string key){
    map<string, int>::iterator it = intData.find(key);
    if (it != intData.end()){
        return intData.at(key);
    }
    else{
        cout << "[ERROR] Key " << key << " not found in int-Cofig" << endl;
        return -1;
    } 
}

float Config::getFloatValue(string key){
    map<string, float>::iterator it = floatData.find(key);
    if (it != floatData.end()){
        return floatData.at(key);
    }
    else{
        cout << "[ERROR] Key " << key << " not found in float-Cofig" << endl;
        return -1;
    } 
}

string Config::getStringValue(string key){
    map<string, string>::iterator it = stringData.find(key);
    if (it != stringData.end()){
        return stringData.at(key);
    }
    else{
        cout << "[ERROR] Key " << key << " not found in string-Cofig" << endl;
        return "_NO_CONFIG_VALUE_";
    } 

}

bool Config::getBoolValue(string key){
    map<string, bool>::iterator it = boolData.find(key);
    if (it != boolData.end()){
        return boolData.at(key);
    }
    else{
        cout << "[ERROR] Key " << key << " not found in bool-Cofig" << endl;
        return false;
    } 

}



void Config::setIntValue(string key, int value){
    map_set(intData, key, value);

}
void Config::setFloatValue(string key, float value){
    map_set(floatData, key, value);
}
void Config::setStringValue(string key, string value){
    map_set(stringData, key, value);
}
void Config::setBoolValue(string key, bool value){
    map_set(boolData, key, value);
}

void Config::saveConfig(){
    cout << "saving config.json" << endl;
    writeConfigFile();
}

