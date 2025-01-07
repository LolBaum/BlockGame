#pragma once

#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include "item.hpp"
//#include "Block.hpp"



// class ItemContainer
// {
// private:
//     /* data */
// public:
//     ItemContainer(/* args */);
//     ~ItemContainer();
// };



class Inventory
{
private:
    unsigned int size = 9;
    /* Item items[9] = {Item(0), Item(0), Item(0), Item(0), Item(0), 
               Item(0), Item(0), Item(0), Item(0), }; */
//    Item items[9] = {Item(1, 300), Item(6, 300), Item(3, 64), Item(4, 64), Item(5, 300),
//               Item(2, 64), Item(7, 64), Item(8, 64), Item(9, 64)};
    Item items[9] = {Item(1, 300), Item(6, 300), Item(3, 64), Item(4, 64), Item(5, 300),
                     Item(10, 64), Item(7, 64), Item(8, 64), Item(9, 64)};
    int first_empty_slot = 0;
public:
    Inventory(/* args */);
    ~Inventory();

    void printInfo();
    Item* get_item(int slot);
    bool contains(int id);

    int pop_item(int slot);

    void add_item(int id, int amount);
    void update_first_empty_slot();
    void update();

	std::string type_info_string();
    std::string info_string();
    std::string improved_info_string(int slot);
};


#endif
