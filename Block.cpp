#include "Block.hpp"
#include <sstream>
#include <vector>

#include "item.hpp"




Block::Block(int id) {
    typeId = id;
    //rotation = id+1;
    //light = id*2;
}
Block::Block() {
    typeId = 0;
}
Block::~Block() {}

int Block::getId() {
    return typeId;
}
void Block::setId(int id) {
    typeId = id;
}







BlockType::BlockType(int TypeId,  std::string blocktypename, TextureType tex_type, 
            int tex_cord_x, int tex_cord_y, TransparencyType opaqueness, bool collision) {
    //std::cout << "adding Blocktype: " << blocktypename << std::endl;
    initialize_basic(TypeId, blocktypename, tex_type, opaqueness, collision);
    texture = StadardBlockTexture(tex_cord_x, tex_cord_y);
    texture_type = tex_type;
    printInfo();
}
BlockType::BlockType(int TypeId, SpecialBlockTexture tex, std::string blocktypename, TextureType tex_type, 
            TransparencyType opaqueness, bool collision) {
    //std::cout << "adding Blocktype: " << blocktypename << std::endl;
    initialize_basic(TypeId, blocktypename, tex_type, opaqueness, collision);
    int tex_cord_x = 0;
    int tex_cord_y = 0;
    tex.get_front(&tex_cord_x, &tex_cord_y);
    texture = StadardBlockTexture(tex_cord_x, tex_cord_y);
    multi_texture = tex;
    texture_type = tex_type;
    printInfo();
}
void BlockType::initialize_basic(int TypeId,  std::string blocktypename, TextureType tex_type, TransparencyType opaqueness, bool collision){
    id = TypeId;
    name = blocktypename;
    texture_type = tex_type;
    opaque = opaqueness;
    has_collision = collision; // blocks with no collision, should have opaqueness = false 
}


int BlockType::GetId() {
    return id;
}
void BlockType::get_tex_coords(int* uv_x, int* uv_y){
    texture.get_coords(uv_x, uv_y);
}
/* 	void load_special_texture(int front_x, int front_y, int left_x, int left_y, int back_x, int back_y, int right_x, int right_y,
                            int top_x, int top_y, int bottom_x, int bottom_y){
    texture = SpecialBlockTexture(front_x, front_y, left_x, left_y, back_x, back_y, right_x, right_y,
                                    top_x, top_y, bottom_x, bottom_y);
} */
TextureType BlockType::get_texture_type(){
    return texture_type;
}
StadardBlockTexture* BlockType::get_texture(){
    return &texture;
}
SpecialBlockTexture* BlockType::get_multi_texture(){
    return &multi_texture;
}

TransparencyType BlockType::isTransparent() {
    return opaque;
}
bool BlockType::hasCollision() {
    return has_collision;
}

void BlockType::printInfo() {
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
std::string BlockType::info_srting(){
    std::stringstream ss;
    std::string output;
    ss << "BlockType: " << id << ", " << name << std::endl;
    output = ss.str();
    return output;
}
std::string BlockType::get_name(){
    return name;
}








BlockType BlockTypeManager::Air = BlockType(0, "Air", SingleTexture, 0, 0, Transparent, false);
std::vector<BlockType> BlockTypeManager::BlockTypeList;

BlockTypeManager::BlockTypeManager() {
    std::cout << "calledConstructor for BlockTypeManager" << std::endl;
}
BlockTypeManager::~BlockTypeManager() {}


void BlockTypeManager::AddBlockType(BlockType type) {
    BlockTypeList.push_back(type);
    ItemTypeManager::AddItemType(new BlockItemType(type.GetId(), type.get_name().append(" block"), type.GetId(), type.get_texture())); // add a function to itm that returns the next free item id
}


BlockType* BlockTypeManager::GetBlockType(int id) {
    BlockType* type_dummy;
    for (int i = 0; i < BlockTypeList.size(); i++) {
        type_dummy = &BlockTypeList.at(i);
        if (type_dummy->GetId() == id) {
            return type_dummy;
        }
    }
    return &Air;
}
