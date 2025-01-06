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

uint8 Block::getRot() {
    return rotation;
}

void Block::setRot(BlockRotation rot) {
    rotation = rot;
}


BlockType::BlockType(int TypeId,  std::string blocktypename, TextureType tex_type,
            int tex_cord_x, int tex_cord_y, TransparencyType opaqueness, bool collision, bool onAir) {
    //std::cout << "adding Blocktype: " << blocktypename << std::endl;
    initialize_basic(TypeId, blocktypename, tex_type, opaqueness, collision, onAir);
    texture = StadardBlockTexture(tex_cord_x, tex_cord_y);
    texture_type = tex_type;
    printInfo();
}
BlockType::BlockType(int TypeId, SpecialBlockTexture tex, std::string blocktypename, TextureType tex_type, 
            TransparencyType opaqueness, bool collision, bool onAir, bool isRot) {
    //std::cout << "adding Blocktype: " << blocktypename << std::endl;
    initialize_basic(TypeId, blocktypename, tex_type, opaqueness, collision, onAir);
    int tex_cord_x = 0;
    int tex_cord_y = 0;
    tex.get_front(&tex_cord_x, &tex_cord_y);
    texture = StadardBlockTexture(tex_cord_x, tex_cord_y);
    multi_texture = tex;
    texture_type = tex_type;
    is_rotatable = isRot;
    printInfo();
}
void BlockType::initialize_basic(int TypeId,  std::string blocktypename, TextureType tex_type,
                                 TransparencyType opaqueness, bool collision, bool onAir){
    id = TypeId;
    name = blocktypename;
    texture_type = tex_type;
    opaque = opaqueness;
    has_collision = collision; // blocks with no collision, should have opaqueness = false
    is_transparent = opaque==Foliage || opaque==Transparent || opaque==Glass;
    is_placeable_on_air = onAir;
    is_rotatable = false;
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

bool BlockType::isTransparent() {
    return is_transparent;
}
bool BlockType::hasCollision() {
    return has_collision;
}
bool BlockType::isPlaceableOnAir() {
    return is_placeable_on_air;
}

void BlockType::printInfo() {
    std::cout << info_string() << std::endl;
}
std::string BlockType::info_string(){
    std::stringstream ss;
    std::string tt;
    std::string opt;
    std::string col;
    std::string onAir;
    std::string isRot;
    if (texture_type == SingleTexture){
        tt = "SingleTexture";
    }else if (texture_type == MultiTexture){
        tt = "MultiTexture";
    }else if (texture_type == Cross){
        tt = "Cross";
    }else{
        tt = "UNDEFINED";
    }

    if (opaque == Solid){
        opt = "Solid";
    } else if (opaque == Foliage){
        opt = "Foliage";
    } else if (opaque == Transparent){
        opt = "Transparent";
    } else{
        tt = "UNDEFINED";
    }

    if (has_collision){
        col = "has collision";
    }else{
        col = "NO collision";
    }

    if (is_placeable_on_air){
        onAir = "is placeable on air";
    }else{
        onAir = "NOT placeable on air";
    }

    if (is_rotatable){
        isRot = "is rotatable";
    }else{
        isRot = "is NOT rotatable";
    }
    std::cout << "BlockType: " << id << ", " << name << ", " << tt << ", " << opt << ", "
              << col << ", " << onAir << ", " << isRot << std::endl;
    return ss.str();
}
std::string BlockType::get_name(){
    return name;
}

TransparencyType BlockType::get_transparency_type() {
    return opaque;
}

bool BlockType::isRotatable() {
    return is_rotatable;
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
