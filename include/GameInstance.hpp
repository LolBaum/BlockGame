//
// Created by Baum on 23.12.2024.
//

#ifndef BLOCKGAME_GAMEINSTANCE_H
#define BLOCKGAME_GAMEINSTANCE_H

#include <iostream>
#include <cmath>
#include <glew.h>

#include "glm/glm.hpp"

#include "definitions.hpp"
#include "renderer.hpp"
#include "shader.hpp"

#include "printFunktions.hpp"
#include "util_funcs.hpp"
#include "text.hpp"
#include "screenshot.hpp"

#include "textures.hpp"

#include "Block.hpp"
#include "Chunk.hpp"
#include "SuperChunk.hpp"

#include "item.hpp"

#include "SDL_handler.hpp"

#include "config.hpp"
#include "local_player_data.hpp"
#include <sstream>

class GameInstance{
private:

    std::string text = "FPS: ";
    float time_since_slow_tick = 1.0;
    float time_since_left_tick = 0;
    float time_since_right_tick = 0;
    int ticks_since_last_jump=0;

    bool show_text = true;
    bool render_wireframe = false;

    bool buttonW = false;
    bool buttonA = false;
    bool buttonS = false;
    bool buttonD = false;
    bool buttonZ = false;
    bool buttonC = false;
    bool buttonSpace = false;
    //bool buttonSpacePress = false;
    bool buttonShift = false;
    bool buttonCtrl = false;
    bool buttonF1 = false;
    bool mouseButtonL = false;
    bool mouseButtonR = false;
    bool mouseButtonClickL = false;
    bool mouseButtonClickR = false;
    bool releasedSpaceSinceJump = false;

    int debug_render_state = 0; // default
    bool graphicalDebug = false;

    bool close = false;
    float time = 0.0f;

public:
    GameInstance() {}
    ~GameInstance() {}

    void start();
    void runGameLoop();
    void stop();

    void initialize();
    void load();

    void handleInput();
    void applyGameMechanics();
    void render();

    Font* font;
    Font* ItemFont; // part of the problem
    LocalPlayer* player;
    Uint64 perfCounterFrequency;
    Uint64 lastCounter;
    float delta;
    Shader* shader;
    Texture* texture;

    Shader* compositeShader;
    Shader* screenShader;
    Shader* UIShader;
    Shader* fontShader;
    Texture* UItexture;
    InventoryMesh* ui;
    Renderer* renderer;

    uint32 ui_projection_uniformLocation;
    uint32 text_projection_uniformLocation;
    glm::mat4 ui_projection;
};
#endif //BLOCKGAME_GAMEINSTANCE_H
