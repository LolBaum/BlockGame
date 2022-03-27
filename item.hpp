#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "textures.hpp"
//#include "Block.hpp"
#include "glm/glm.hpp"

#include "SuperChunk.hpp"

//extern SuperChunk chunkManger;
enum ItemGroup {Normal, BlockItem};


class Item{
private:
    int typeId;
    int amount = 1;
public:
    Item(int id, int item_amount){
        typeId = id;
        amount = item_amount;
    }
    Item(int id){
        typeId = id;
    }
    Item(){
        typeId = 0;
    }
    ~Item(){}

    int getId(){
        return typeId;
    }
    int getAmount(){
        return amount;
    }
    void setId(int id){
        typeId = id;
    }
    void add(int number){
        amount += number;
    }
    void remove(int number){
        amount -= number;
        if (amount <= 0){
            amount = 0;
            typeId = 0;
        }
    }
};

class ItemType{
protected:
    int id;
	std::string name;
    ItemGroup group = Normal;
	//StadardBlockTexture texture = StadardBlockTexture();
public:
    ItemType(int TypeID, std::string TypeName){
        id = TypeID;
        name = TypeName;
        group = Normal;
        
    }
    ~ItemType(){}

    virtual void printInfo() {
		std::cout << "ItemType: " << id << ", " << name << std::endl;
	}
	virtual std::string info_string(){
		std::stringstream ss;
		string output;
		ss << "ItemType: " << id << ", " << name << std::endl;
		output = ss.str();
		return output;
	}
    int getId(){
        return id;
    }
    std::string getName(){
        return name;
    }
    virtual void leftClickFunc(){}
    virtual void rightClickFunc(glm::vec3 position, int* return_id){}
};

class BlockItemType : public ItemType
{
protected:
    int blockTypeId;
public:
    BlockItemType(int TypeID, std::string TypeName, int BlockTypeId) : ItemType(TypeID, TypeName){
        blockTypeId = BlockTypeId;
        group = BlockItem;
    }
    void printInfo() {
		std::cout << "BlockItemType: " << id << ", " << name << ", " << blockTypeId << std::endl;
	}
    void rightClickFunc(glm::vec3 position, int* return_id){
        *return_id = blockTypeId;
        //chunkManager.setBlock(position, blockTypeId);
    }
};




class ItemTypeManager{
private:
public:
    std::vector<ItemType*> ItemTypeList;
	ItemType noItemtype = ItemType(0, "nothing");
    Item noItem = Item(0, 0);
    ItemTypeManager() {}
    ~ItemTypeManager() {}
    void AddItemType(ItemType* type) {
		ItemTypeList.push_back(type);
        std::cout << "Added: ";
        type->printInfo();
	}


    ItemType* GetItemType(int id) {
		ItemType* type_dummy;
		for (int i = 0; i < ItemTypeList.size(); i++) {
			type_dummy = ItemTypeList.at(i);
			if (type_dummy->getId() == id) {
				return type_dummy;
			}
		}
	return &noItemtype;
	}
};

ItemTypeManager itm = ItemTypeManager();

