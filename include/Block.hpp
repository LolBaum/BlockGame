#pragma once
#include <vector>
#include <string>

#include "textures.hpp"



enum TextureType { SingleTexture, MultiTexture, Cross };
enum TransparencyType { Solid, Foliage, Glass, Transparent };
enum BlockRotation : uint8 {X_positive, X_negative, Y_positive, Y_negative, Z_positive, Z_negative};

struct Block{
public:
	Block(int id);
	Block();
	~Block();

	int getId();
	void setId(int id);

    uint8 getRot();
    void setRot(BlockRotation rot);

private:
	uint16 typeId;
	uint8 rotation;
	uint8 light;
	uint32 _placeholder_;
	
};


class BlockType {
public:
	BlockType(int TypeId,  std::string blocktypename="Unamed Block", TextureType tex_type = SingleTexture, 
			  int tex_cord_x = 0, int tex_cord_y = 0, TransparencyType opaqueness = Solid, bool collision=true,
              bool onAir=true);

	BlockType(int TypeId, SpecialBlockTexture tex, std::string blocktypename="Unamed Block", TextureType tex_type = SingleTexture, 
		      TransparencyType opaqueness = Solid, bool collision=true, bool is_placeable_on_air=true, bool isRot=false);
	void initialize_basic(int TypeId,  std::string blocktypename, TextureType tex_type, TransparencyType opaqueness,
                          bool collision, bool onAir);
	

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
    bool isPlaceableOnAir();
    bool isRotatable();

    TransparencyType get_transparency_type();

	void printInfo();
	std::string info_string();
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
    bool is_placeable_on_air = true;
    bool is_rotatable = false;

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

