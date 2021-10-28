#pragma once
#include <vector>

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
	BlockType(int id, int tex_cord_x = 0, int tex_cord_y = 0) {
		typeId = id;
		tex_x = tex_cord_x;
		tex_y = tex_cord_y;
	}

	int GetId() {
		return typeId;
	}
	int isOpaque() {
		return opaque;
	}

	void printInfo() {
		std::cout << "id: " << typeId << std::endl;
		std::cout << "tex_x: " << tex_x << std::endl;
		std::cout << "tex_y: " << tex_y << std::endl;
	}

private:
	int typeId;
	int tex_x;
	int tex_y;
	
	bool opaque = 0;
};







class BlockTypeManager{
public:
	std::vector<BlockType> BlockTypeList;
	
	BlockTypeManager() {

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
	}

	
}; 
BlockTypeManager btm = BlockTypeManager();

