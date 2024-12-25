#include "world.hpp"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "config.hpp"
#include "version.hpp"
#include "printFunktions.hpp"


#include "inventory.hpp"



using namespace std;
using json = nlohmann::json;


json vec3ToJson(glm::vec3 v){
    json jvec;
    jvec["x"] = v.x;
    jvec["y"] = v.y;
    jvec["z"] = v.z;
    return jvec;
}

glm::vec3 jsonTovec3(json j){
    glm::vec3 v = glm::vec3(j["x"], j["y"], j["z"]);
    return v;
}



template<typename type>
void print_map(string comment, const map<string, type>& m){
    cout << comment ;
    for (const auto& n : m) {
        cout << n.first << " = " << n.second << "; ";
    }
    cout << '\n';
}

void print_map(string comment, const map<string, glm::vec3>& m){
    cout << comment ;
    for (const auto& n : m) {
        cout << n.first << " = " << vec3_toString(n.second) << "; ";
    }
    cout << '\n';
}

template<typename type>
void add_map_to_json(json& j, const map<string, type>& m){
    for (const auto& n : m) {
        j[n.first] = n.second;
    }
}

void add_vec3map_to_json(json& j, const map<string, glm::vec3>& m){
    json j_vec_group;
    for (const auto& n : m) {
        j_vec_group[n.first] = vec3ToJson(n.second);
    }
    j["vec3"] = j_vec_group;
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

void map_set_multiple_vec3(map<string, glm::vec3>& m, json j){
    cout << "1:    " << j << endl;
    for (auto& el : j["vec3"].items()){
        cout << el << endl;
        map_set(m, el.key(), jsonTovec3(el.value()));
    }

}

map<string, int> World::intData;
map<string, float> World::floatData;
map<string, string> World::stringData;
map<string, bool> World::boolData;
map<string, glm::vec3> World::vec3Data;
//map<string, Inventory> World::containerData;
std::string World::filePaht;



World::World(){}

void World::init(){
    string fpath = Config::getStringValue("WorldSavePath");
    if (fpath.length() > 0){
        fpath = fpath + "/";
    }
    filePaht = fpath + "worldData.json";

    intData.insert(pair<string,int> (string("Seed"), 123456789));

    vec3Data.insert(pair<string,glm::vec3> (string("PlayerPosition"), glm::vec3(8.0f, 48.0f, -100.0f)));
    vec3Data.insert(pair<string,glm::vec3> (string("PlayerSpawnPoint"), glm::vec3(8.0f, 60.0f, -100.0f)));
    vec3Data.insert(pair<string,glm::vec3> (string("PlayerLookAt"), glm::vec3(8.0f, 30.0f, -100.0f)));

    print_map("[World-info] Vec3: ", vec3Data);

    //print_map("[World-info] int: ", intData);
    //print_map("[World-info] string: ", stringData);
    //print_map("[World-info] float: ", floatData);
    //print_map("[World-info] bool: ", boolData);

    loadWorldFile();

    //print_map("[World-info] int: ", intData);
    //print_map("[World-info] string: ", stringData);

}

void World::locateWorldFile(){}

void World::loadWorldFile(){
    string line;
    string file_name = filePaht;
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
            case json::value_t::object:
                //map_set(boolData, el.key(), (bool)el.value());
                if (el.key() == "vec3"){
                    //cout << "Loading a Vec3: " << el.key() << " value " << el.value() << " is type " <<  el.value().type_name() << endl;
                    map_set_multiple_vec3(vec3Data, el);
                }
                else if (el.key() == "container"){
                    cout << "Loading a Container: " << el.key() << " value " << el.value() << endl;
                    //map_set_multiple_vec3(vec3Data, el);
                }
                break;
            
            default:
                cout << "[World] loading : " << el.key() << " value " << el.value() << " is type " <<  el.value().type_name() << " which is not implemented yet" << endl;
                break;
            }
            //std::cout << "key: " << el.key() << ", value:" << el.value() << ", type: " << el.value().type_name() << '\n';
        }
    }
    else {
        cout << "Unable to open World file: " << file_name << endl;
    } 
}

void World::writeWorldFile(){

    string file_name = filePaht;
    ofstream file (file_name);

    json j;
    add_map_to_json(j, intData);
    add_map_to_json(j, stringData);
    add_map_to_json(j, floatData);
    add_map_to_json(j, boolData);
    add_vec3map_to_json(j, vec3Data);


    if (file.is_open())
    {
        file << j.dump(4);
        file.close();
    }
    else cout << "Unable to open file " << file_name << endl;
}

int World::getIntValue(string key){
    map<string, int>::iterator it = intData.find(key);
    if (it != intData.end()){
        return intData.at(key);
    }
    else{
        cout << "[ERROR] Key " << key << " not found in int-Config" << endl;
        return -1;
    } 
}

float World::getFloatValue(string key){
    map<string, float>::iterator it = floatData.find(key);
    if (it != floatData.end()){
        return floatData.at(key);
    }
    else{
        cout << "[ERROR] Key " << key << " not found in float-Config" << endl;
        return -1;
    } 
}

string World::getStringValue(string key){
    map<string, string>::iterator it = stringData.find(key);
    if (it != stringData.end()){
        return stringData.at(key);
    }
    else{
        cout << "[ERROR] Key " << key << " not found in string-Config" << endl;
        return "_NO_World_VALUE_";
    } 

}

bool World::getBoolValue(string key){
    map<string, bool>::iterator it = boolData.find(key);
    if (it != boolData.end()){
        return boolData.at(key);
    }
    else{
        cout << "[ERROR] Key " << key << " not found in bool-Config" << endl;
        return false;
    } 

}


glm::vec3 World::getvec3Value(string key){
    auto it = vec3Data.find(key);
    if (it != vec3Data.end()){
        return vec3Data.at(key);
    }
    else{
        cout << "[ERROR] Key " << key << " not found in vec3-Config" << endl;
        return glm::vec3(0,0,0);
    } 

}



void World::setIntValue(string key, int value){
    map_set(intData, key, value);

}
void World::setFloatValue(string key, float value){
    map_set(floatData, key, value);
}
void World::setStringValue(string key, string value){
    map_set(stringData, key, value);
}
void World::setBoolValue(string key, bool value){
    map_set(boolData, key, value);
}

void World::setvec3Value(string key, glm::vec3 value){
    map_set(vec3Data, key, value);
}

void World::saveWorldFile(){
    cout << "saving " << filePaht << endl;
    writeWorldFile();
}

