#include "util_funcs.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
    #include <direct.h>
#endif
#include <iostream>


bool exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

bool isDir(const std::string& name){
	struct stat info;   
  	return (info.st_mode & S_IFDIR); 
}

void createDir(const std::string& name){
	#ifdef __linux__ 
		mode_t mode = 0755; // LOL WHY?
		mkdir(name.c_str(), mode); // EDIT
	#elif _WIN32
		_mkdir(name.c_str());
	#endif
}

void createDir_IfDoesNotExist(const std::string& name){
	if ( ! isDir(name)){
		std::cout << "creating Dir: " << name << std::endl; 
		createDir(name);
	}
}



bool compareVec3(glm::vec3 v1, glm::vec3 v2) {
	return ((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z));
}

bool isChunkEmty(glm::vec3 v1) {
	return ((v1.x == -1) && (v1.y == -1) && (v1.z == -1));
}

glm::vec3 Chunk_Position(glm::vec3 pos) {
	glm::vec3 chunkpos;
	chunkpos.x = floor(pos.x / 16) * 16;
	chunkpos.y = floor(pos.y / 16) * 16;
	chunkpos.z = floor(pos.z / 16) * 16;
	return chunkpos;
}
glm::vec3 position_in_chunk(glm::vec3 pos) {
	int x = positionInChunk(pos.x);
    int y = positionInChunk(pos.y);
    int z = positionInChunk(pos.z);
	return {x, y, z};
}

int positionInChunk(float p){
    int x;
    if (p >= 0) {
        x = abs((int)floor(p) % 16); // USE FLOOR !
    }
    else {
        if ((int)floor(p) % 16 == 0){
            x = 0;
        }else{
            x = 16 - abs((int)floor(p) % 16);
        }
    }
    return x;
}


int positionInChunk(int p){
    int x;
    if (p >= 0) {
        x = abs((int)p % CX); // not floor(). use (int)
    }
    else {
        x = CX -1 - abs((int)p % CX);
    }
    return x;
}

glm::vec3 scale_vec3(glm::vec3 vec, float value) {
	vec.x *= value;
	vec.y *= value;
	vec.z *= value;
	return vec;
}
bool is_inside_block(glm::vec3 blockpos, glm::vec3 pos){
	blockpos = floor(blockpos);
	pos = floor(pos);
	if (blockpos == pos){
		return true;
	}
	else{
		return false;
	}
}

inline bool positive(int a){
    return a>=0;
}

unsigned long long map_3d_to_1d(glm::vec3 v){
    int x = (int) v.x / CX;
    int y = (int) v.y / CY;
    int z = (int) v.z / CZ;

    int s1 = positive(x);
    int s2 = positive(y);
    int s3 = positive(z);

    x = abs(x);
    y = abs(y);
    z = abs(z);

    unsigned long long T = (x + y) * (x + y + 1) / 2 + y;
    unsigned long long T2 =  (T + z) * (T + z + 1) / 2 + z;
    return T2*8 + s1 + s2*2 + s3*4;
}

void calcChunkCoords(glm::vec3 pos, int* x, int* y, int* z){
    *x = toChunkCoord(pos.x);
    *y = toChunkCoord(pos.y);
    *z = toChunkCoord(pos.z);
}

inline int toChunkCoord(float p){
    int c;
    if (p>=0){
        c = floor(p) - (int)floor(p)%16;
    }
    else{
        c =  floor(p) - (16 + ((int)floor(p)+1)%16)+1;
    }
    return c;
}

bool compareGreaterVec3(glm::vec3 a, glm::vec3 b) {
    if (a.x > b.x){
        return true;
    }else if(a.x < b.x){
        return false;
    }

    if (a.y > b.y){
        return true;
    }else if(a.y < b.y){
        return false;
    }

    if (a.z > b.z){
        return true;
    }else{
        return false;
    }
}

std::ostream &operator<<(std::ostream &os, const glm::vec3 &m) {
    return os << m.x << " "  << m.y << " "  << m.z;
}

std::ostream &operator<<(std::ostream &os, const std::vector<glm::vec3> &m) {
    for (auto x: m) { // TODO: check for size correctly!
        os << x << std::endl;
    };
}
