#pragma once
#include "Block.hpp"

class ItemContainer
{
private:
    /* data */
public:
    ItemContainer(/* args */);
    ~ItemContainer();
};






class Inventory
{
private:
    unsigned int size = 9;
    /* Item items[9] = {Item(0), Item(0), Item(0), Item(0), Item(0), 
               Item(0), Item(0), Item(0), Item(0), }; */
    Item items[9] = {Item(0,0), Item(0,0), Item(3, 64), Item(4, 64), Item(5, 64), 
               Item(0,0), Item(7, 64), Item(8, 64), Item(9, 64)};
    int first_empty_slot = 0;
public:
    Inventory(/* args */){}
    ~Inventory(){}

    void printInfo() {
        for (int i=0; i<size; i++){
            itm.GetItemType(items[i].getId())->printInfo();
        }
	}
    Item* get_item(int slot){
        slot--;
        if (slot >= 0 && slot < size){
            return &items[slot];
        }
        return &itm.noItem;
    }
    bool contains(int id){
        for (int i=0; i<size; i++){
            if(items[i].getId() == id){
                return true;
            }
        }
        return false;
    }
    // void remove_item(int id){
    //     for (int i=0; i<size; i++){
    //         if(items[i].getId() == id){
    //             items[i].remove(1);
    //         }
    //     }
    // }
    int pop_item(int slot){
        int type = 0;
        slot--;
        if (slot >= 0 && slot < size){
            type = items[slot].getId();
            items[slot].remove(1);
        }
        update();
        return type;
    }

    void add_item(int id, int amount){
        bool found = false;
        for (int i=0; i<size; i++){
            if(items[i].getId() == id){
                items[i].add(amount);
                found = true;
                break;
            }
        }
        if (!found){
            if (first_empty_slot != -1){
                items[first_empty_slot] = Item(id, amount);
            }
        }
        update();
    }
    void update_first_empty_slot(){
        first_empty_slot = -1;
        for (int i=0; i<size; i++){
            if(items[i].getId() == 0){
                first_empty_slot = i;
                break;
            }
        }
    }
    void update(){
        update_first_empty_slot();
    }

	std::string type_info_string(){
		std::stringstream ss;
		string output;
        for (int i=0; i<size; i++){
            ss << itm.GetItemType(items[i].getId())->info_string();
        }
		output = ss.str();
		return output;
	}
    std::string info_string(){
		std::stringstream ss;
		string output;
        for (int i=0; i<size; i++){
            ss << i << ": " << itm.GetItemType(items[i].getId())->getName() << " " << items[i].getAmount() << std::endl;
        }
		output = ss.str();
		return output;
	}
    std::string improved_info_string(int slot){
		std::stringstream ss;
		string output;
        for (int i=0; i<size; i++){
            if (i+1 == slot){
                ss << ">>> " ;
            }
            if (items[i].getId() != 0){
                ss << i+1 << ": " << itm.GetItemType(items[i].getId())->getName() << " " << items[i].getAmount() << std::endl;
            }
            else{
                ss << i+1 << ": " << std::endl;
            }
            
        }
		output = ss.str();
		return output;
	}
};



