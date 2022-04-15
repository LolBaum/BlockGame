#include "item.hpp"
#include <string>
#include <sstream>
#include <vector>
#include "glm/glm.hpp"
//#include "SuperChunk.hpp"




Item::Item(int id, int item_amount){
    typeId = id;
    amount = item_amount;
}
Item::Item(int id){
    typeId = id;
}
Item::Item(){
    typeId = 0;
}
Item::~Item(){}

int Item::getId(){
    return typeId;
}
int Item::getAmount(){
    return amount;
}
void Item::setId(int id){
    typeId = id;
}
void Item::add(int number){
    amount += number;
}
void Item::remove(int number){
    amount -= number;
    if (amount <= 0){
        amount = 0;
        typeId = 0;
    }
}



ItemType::ItemType(int TypeID, std::string TypeName){
    id = TypeID;
    name = TypeName;
    group = Normal;
    
}
ItemType::~ItemType(){}

void ItemType::printInfo() {
    std::cout << "ItemType: " << id << ", " << name << std::endl;
}
std::string ItemType::info_string(){
    std::stringstream ss;
    std::string output;
    ss << "ItemType: " << id << ", " << name << std::endl;
    output = ss.str();
    return output;
}
int ItemType::getId(){
    return id;
}
std::string ItemType::getName(){
    return name;
}
void ItemType::leftClickFunc(){}
void ItemType::get_tex_coords(int* uv_x, int* uv_y){}
void ItemType::rightClickFunc(glm::vec3 position, int* return_id){}



    BlockItemType::BlockItemType(int TypeID, std::string TypeName, int BlockTypeId, StadardBlockTexture* tex) : ItemType(TypeID, TypeName){
        blockTypeId = BlockTypeId;
        group = BlockItem;
        texture = tex;
    }
    void BlockItemType::printInfo() {
		std::cout << "BlockItemType: " << id << ", " << name << ", " << blockTypeId << std::endl;
	}
    void BlockItemType::rightClickFunc(glm::vec3 position, int* return_id){
        *return_id = blockTypeId;
        //SuperChunk::setBlock(position, blockTypeId);
    }
    void BlockItemType::get_tex_coords(int* uv_x, int* uv_y){
		texture->get_coords(uv_x, uv_y);
	}






ItemType ItemTypeManager::noItemtype = ItemType(0, "nothing");
Item ItemTypeManager::noItem = Item(0, 0);
std::vector<ItemType *> ItemTypeManager::ItemTypeList;

ItemTypeManager::ItemTypeManager() {}
ItemTypeManager::~ItemTypeManager() {}

void ItemTypeManager::AddItemType(ItemType* type) {
    ItemTypeList.push_back(type);
    std::cout << "Added: ";
    type->printInfo();
}


ItemType* ItemTypeManager::GetItemType(int id) {
    ItemType* type_dummy;
    for (int i = 0; i < ItemTypeList.size(); i++) {
        type_dummy = ItemTypeList.at(i);
        if (type_dummy->getId() == id) {
            return type_dummy;
        }
    }
return &noItemtype;
}


