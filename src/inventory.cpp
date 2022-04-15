#include "inventory.hpp"
#include "item.hpp"
#include <iostream>
#include <sstream>





Inventory::Inventory(/* args */){}
Inventory::~Inventory(){}

void Inventory::printInfo() {
    for (int i=0; i<size; i++){
        ItemTypeManager::GetItemType(items[i].getId())->printInfo();
    }
}
Item* Inventory::get_item(int slot){
    slot--;
    if (slot >= 0 && slot < size){
        return &items[slot];
    }
    return &ItemTypeManager::noItem;
}
bool Inventory::contains(int id){
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
int Inventory::pop_item(int slot){
    int type = 0;
    slot--;
    if (slot >= 0 && slot < size){
        type = items[slot].getId();
        items[slot].remove(1);
    }
    update();
    return type;
}

void Inventory::add_item(int id, int amount){
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
void Inventory::update_first_empty_slot(){
    first_empty_slot = -1;
    for (int i=0; i<size; i++){
        if(items[i].getId() == 0){
            first_empty_slot = i;
            break;
        }
    }
}
void Inventory::update(){
    update_first_empty_slot();
}

std::string Inventory::type_info_string(){
    std::stringstream ss;
    std::string output;
    for (int i=0; i<size; i++){
        ss << ItemTypeManager::GetItemType(items[i].getId())->info_string();
    }
    output = ss.str();
    return output;
}
std::string Inventory::info_string(){
    std::stringstream ss;
    std::string output;
    for (int i=0; i<size; i++){
        ss << i << ": " << ItemTypeManager::GetItemType(items[i].getId())->getName() << " " << items[i].getAmount() << std::endl;
    }
    output = ss.str();
    return output;
}
std::string Inventory::improved_info_string(int slot){
    std::stringstream ss;
    std::string output;
    for (int i=0; i<size; i++){
        if (i+1 == slot){
            ss << ">>> " ;
        }
        if (items[i].getId() != 0){
            ss << i+1 << ": " << ItemTypeManager::GetItemType(items[i].getId())->getName() << " " << items[i].getAmount() << std::endl;
        }
        else{
            ss << i+1 << ": " << std::endl;
        }
        
    }
    output = ss.str();
    return output;
}





