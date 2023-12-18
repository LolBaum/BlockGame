#pragma once
#include <vector>
#include <string>
#include "textures.hpp"
//#include "Block.hpp"
#include "glm/glm.hpp"



//extern SuperChunk chunkManger;
enum ItemGroup {Normal, BlockItem};


class Item{
private:
    int typeId;
    int amount = 1;
public:
    Item(int id, int item_amount);
    Item(int id);
    Item();
    ~Item();

    int getId();
    int getAmount();
    void setId(int id);
    void add(int number);
    void remove(int number);
};

class ItemType{
protected:
    int id;
	std::string name;
    ItemGroup group = Normal;
	//StadardBlockTexture texture = StadardBlockTexture();
public:
    ItemType(int TypeID, std::string TypeName);
    ~ItemType();

    virtual void printInfo();
	virtual std::string info_string();
    int getId();
    std::string getName();
    virtual void leftClickFunc();
    virtual void get_tex_coords(int* uv_x, int* uv_y);
    virtual void rightClickFunc(glm::vec3 position, int* return_id);
};

class BlockItemType : public ItemType
{
protected:
    int blockTypeId;
    StadardBlockTexture* texture;
public:
    BlockItemType(int TypeID, std::string TypeName, int BlockTypeId, StadardBlockTexture* tex);
    ~BlockItemType();
    void printInfo();
    void rightClickFunc(glm::vec3 position, int* return_id);
    void get_tex_coords(int* uv_x, int* uv_y);
};




class ItemTypeManager{
private:
    ItemTypeManager();
    ~ItemTypeManager();
public:
    static std::vector<ItemType*> ItemTypeList;
	static ItemType noItemtype;
    static Item noItem;
    
    static void AddItemType(ItemType* type);

    static ItemType* GetItemType(int id);
};


