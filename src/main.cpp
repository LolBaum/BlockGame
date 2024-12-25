#include "GameInstance.hpp"
#include <iostream>


int main(int argc, char** argv) {
    std::cout << "Starting Program" << std::endl;

    GameInstance game;
    game.start();

    std::cout << "Program exits" << std::endl;
    return 0;
}