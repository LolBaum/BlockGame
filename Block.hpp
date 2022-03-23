#pragma once
#include <vector>
#include <string>
#include "textures.hpp"
#include <sstream>

enum TextureType { SingleTexture, MultiTexture, blue };
enum TransparencyType { Solid, Transparent_opaque, Transparent };

struct Block{
public:
	Block(int id) {
		typeId = id;
		//rotation = id+1;
		//light = id*2;
	}
	Block() {
		typeId = 0;
	}
	~Block() {}

	int getId() {
		return typeId;
	}
	void setId(int id) {
		typeId = id;
	}

private:
	int typeId;
	//int rotation;
	//int light;
};


class BlockType {
public:
	BlockType(int TypeId,  std::string blocktypename="Unamed Block", TextureType tex_type = SingleTexture, 
			  int tex_cord_x = 0, int tex_cord_y = 0, TransparencyType opaqueness = Solid, bool collision=true) {
		//std::cout << "adding Blocktype: " << blocktypename << std::endl;
		initialize_basic(TypeId, blocktypename, tex_type, opaqueness, collision);
		texture = StadardBlockTexture(tex_cord_x, tex_cord_y);
		texture_type = tex_type;
		printInfo();
	}
	BlockType(int TypeId, SpecialBlockTexture tex, std::string blocktypename="Unamed Block", TextureType tex_type = SingleTexture, 
		      TransparencyType opaqueness = Solid, bool collision=true) {
		//std::cout << "adding Blocktype: " << blocktypename << std::endl;
		initialize_basic(TypeId, blocktypename, tex_type, opaqueness, collision);
		multi_texture = tex;
		texture_type = tex_type;
		printInfo();
	}
	void initialize_basic(int TypeId,  std::string blocktypename, TextureType tex_type, TransparencyType opaqueness, bool collision){
		id = TypeId;
		name = blocktypename;
		texture_type = tex_type;
		opaque = opaqueness;
		has_collision = collision; // blocks with no collision, should have opaqueness = false 
	}
	

	int GetId() {
		return id;
	}
	void get_tex_coords(int* uv_x, int* uv_y){
		texture.get_coords(uv_x, uv_y);
	}
/* 	void load_special_texture(int front_x, int front_y, int left_x, int left_y, int back_x, int back_y, int right_x, int right_y,
							  int top_x, int top_y, int bottom_x, int bottom_y){
		texture = SpecialBlockTexture(front_x, front_y, left_x, left_y, back_x, back_y, right_x, right_y,
							          top_x, top_y, bottom_x, bottom_y);
	} */
	TextureType get_texture_type(){
		return texture_type;
	}
	StadardBlockTexture* get_texture(){
		return &texture;
	}
	SpecialBlockTexture* get_multi_texture(){
		return &multi_texture;
	}

	TransparencyType isTransparent() {
		return opaque;
	}
	bool hasCollision() {
		return has_collision;
	}

	void printInfo() {
		std::string tt;
		std::string opt;
		if (texture_type == SingleTexture){
			tt = "SingleTexture";
		}else if (texture_type == MultiTexture){
			tt = "MultiTexture";
		}

		if (opaque == Solid){
			opt = "Solid";
		} else if (opaque == Transparent_opaque){
			opt = "Transparent_opaque";
		} else if (opaque == Transparent){
			opt = "Transparent";
		} 
		std::cout << "BlockType: " << id << ", " << name << ", " << tt << ", " << opt << std::endl;
	}
	std::string info_srting(){
		std::stringstream ss;
		string output;
		ss << "BlockType: " << id << ", " << name << std::endl;
		output = ss.str();
		return output;
	}

private:
	int id;
	std::string name;
	StadardBlockTexture texture = StadardBlockTexture();
	SpecialBlockTexture multi_texture = SpecialBlockTexture();
	TextureType texture_type;
	
	TransparencyType opaque = Solid;
	bool has_collision = true;
};







class BlockTypeManager{
public:
	std::vector<BlockType> BlockTypeList;
	BlockType Air = BlockType(0, "Air", SingleTexture, 0, 0, Transparent, false);
	
	BlockTypeManager() {
		std::cout << "calledConstructor for BlockTypeManager" << std::endl;
	}
	~BlockTypeManager() {}
	

	void AddBlockType(BlockType type) {
		BlockTypeList.push_back(type);
	}


	BlockType* GetBlockType(int id) {
		BlockType* type_dummy;
		for (int i = 0; i < BlockTypeList.size(); i++) {
			type_dummy = &BlockTypeList.at(i);
			if (type_dummy->GetId() == id) {
				return type_dummy;
			}
		}
	return &Air;
	}

	

	
}; 
BlockTypeManager btm = BlockTypeManager();

