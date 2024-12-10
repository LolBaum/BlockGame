#pragma once
#include <vector>
#include <string>

#include "textures.hpp"



enum TextureType { SingleTexture, MultiTexture, blue };
enum TransparencyType { Solid, Transparent_opaque, Transparent };

struct Block{
public:
	Block(int id);
	Block();
	~Block();

	int getId();
	void setId(int id);
private:
	uint16 typeId;
	uint8 rotation;
	uint8 light;
	uint32 _placeholder_;
	
};


class BlockType {
public:
	BlockType(int TypeId,  std::string blocktypename="Unamed Block", TextureType tex_type = SingleTexture, 
			  int tex_cord_x = 0, int tex_cord_y = 0, TransparencyType opaqueness = Solid, bool collision=true);

	BlockType(int TypeId, SpecialBlockTexture tex, std::string blocktypename="Unamed Block", TextureType tex_type = SingleTexture, 
		      TransparencyType opaqueness = Solid, bool collision=true);
	void initialize_basic(int TypeId,  std::string blocktypename, TextureType tex_type, TransparencyType opaqueness, bool collision);
	

	int GetId();
	void get_tex_coords(int* uv_x, int* uv_y);
/* 	void load_special_texture(int front_x, int front_y, int left_x, int left_y, int back_x, int back_y, int right_x, int right_y,
							  int top_x, int top_y, int bottom_x, int bottom_y){
		texture = SpecialBlockTexture(front_x, front_y, left_x, left_y, back_x, back_y, right_x, right_y,
							          top_x, top_y, bottom_x, bottom_y);
	} */
	TextureType get_texture_type();
	StadardBlockTexture* get_texture();
	SpecialBlockTexture* get_multi_texture();

	bool isTransparent();
	bool hasCollision();

    TransparencyType get_transparency_type();

	void printInfo();
	std::string info_srting();
	std::string get_name();

private:
	int id;
	std::string name;
	StadardBlockTexture texture = StadardBlockTexture();
	SpecialBlockTexture multi_texture = SpecialBlockTexture();
	TextureType texture_type;
	
	TransparencyType opaque = Solid;
	bool has_collision = true;
    bool is_transparent = false;
};







class BlockTypeManager{
	BlockTypeManager();
	~BlockTypeManager();
public:
	static std::vector<BlockType> BlockTypeList;
	static BlockType Air;
	
	static void AddBlockType(BlockType type);
	static BlockType* GetBlockType(int id);
}; 

