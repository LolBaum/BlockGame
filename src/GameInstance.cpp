//
// Created by Baum on 23.12.2024.
//
#include "GameInstance.hpp"



void GameInstance::start() {
    initialize();
    load();
    runGameLoop();
}

void GameInstance::stop() {
    SuperChunk::saveWorld();

    GLCALL(glDeleteTextures(1, texture->get_textureId_ptr()));


    player->setPlayerValues_inWorldData();

    World::saveWorldFile();
    Config::saveConfig();

}

void GameInstance::initialize() {
    Config::init();
    SDL_handler::initialize();

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        std::cout << "Press any Key to exit." << std::endl;
        std::cin.get();
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;


    //        Wichtig
#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openGLDebugCallback, 0);
#endif

    font = new Font();
    ItemFont = new Font();

    std::cout << "screen size: width = " << SDL_handler::getWidth() << " height = " << SDL_handler::getHeight() << std::endl;

    World::init();


    SuperChunk::initialize();



    player = new LocalPlayer();

    shader = &SuperChunk::get_shader();//shader("shaders/basic.vs", "shaders/basic.fs");
    texture = SuperChunk::get_tile_atlas();
    shader->bind();

    perfCounterFrequency = SDL_GetPerformanceFrequency();
    lastCounter = SDL_GetPerformanceCounter();
    delta = 0.0f;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    compositeShader = new Shader("shaders/composite.vs", "shaders/composite.fs");
    screenShader = new Shader("shaders/frame.vs", "shaders/frame.fs");
    fontShader = new Shader("shaders/font.vs", "shaders/font.fs");
    UIShader = new Shader("shaders/UI.vs", "shaders/UI.fs");

    renderer = new Renderer();

    UItexture = new Texture("graphics/UI.png");

    ui = new InventoryMesh();
    ui->update();

    SDL_handler::SetRelativeMouseMode(true); // in Init ?
}


void GameInstance::load() {

    //BlockTypeManager::AddBlockType(BlockType(1, "Geisterstein", SingleTexture, 0, 1,Solid,false));
    BlockTypeManager::AddBlockType(BlockType(1, "Stone", SingleTexture, 0, 1));
    BlockTypeManager::AddBlockType(BlockType(2, "Dirt", SingleTexture, 1, 0));
    BlockTypeManager::AddBlockType(BlockType(3, SpecialBlockTexture(2,0, 2,0, 2,0, 2,0, 3,0, 1,0), "Dirt/Grass", MultiTexture));
    BlockTypeManager::AddBlockType(BlockType(4, "Grass", Cross, 2, 1, Foliage, false, false));
    //BlockTypeManager::AddBlockType(BlockType(5, SpecialBlockTexture(0,2, 0,2, 0,2, 0,2, 1,2, 1,2), "Wood Log", MultiTexture, Solid, true, true, true));
    BlockTypeManager::AddBlockType(BlockType(5, SpecialBlockTexture(1,2, 0,2, 1,2, 0,2, 0,2, 0,2), "Wood Log", MultiTexture, Solid, true, true, AxisAlignedRotation));
    BlockTypeManager::AddBlockType(BlockType(6, "Wooden Planks", SingleTexture, 0, 3));
    //BlockTypeManager::AddBlockType(BlockType(6, "Withe Test - no collision", SingleTexture, 3, 1, Solid, false));
    BlockTypeManager::AddBlockType(BlockType(7, "Leafs", SingleTexture, 2, 3, Foliage));
    BlockTypeManager::AddBlockType(BlockType(8, "Glass", SingleTexture, 3, 2, Glass));
    BlockTypeManager::AddBlockType(BlockType(9, "Ice", SingleTexture, 3, 3, Transparent));
//    BlockTypeManager::AddBlockType(BlockType(10, SpecialBlockTexture(5,15, 0,15, 4,15, 1,15, 3,15, 2,15), "Directional Debug", MultiTexture, Solid, true, true, true));
    BlockTypeManager::AddBlockType(BlockType(10, SpecialBlockTexture(5,14, 0,14, 4,14, 1,14, 3,14, 2,14), "Directional Debug", MultiTexture, Solid, true, true, AxisAlignedRotation));

    ItemTypeManager::AddItemType(new ItemType(10, "Stick"));
}

void GameInstance::handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            close = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    close = true;
                case SDLK_w:
                    buttonW = true;
                    break;
                case SDLK_a:
                    buttonA = true;
                    break;
                case SDLK_s:
                    buttonS = true;
                    break;
                case SDLK_d:
                    buttonD = true;
                    break;
                case SDLK_y:
                    show_text = !show_text;
                    break;
                case SDLK_c:
                    buttonC = true;
                    break;
                case SDLK_g:
                    graphicalDebug = !graphicalDebug;
                    break;
                case SDLK_SPACE:
                    buttonSpace = true;
                    //buttonSpacePress = true;
                    //std::cout << "pressed Space" << std::endl;
                    break;
                case SDLK_LSHIFT:
                    buttonShift = true;
                    break;
                case SDLK_LCTRL:
                    buttonCtrl = true;
                    break;
                case SDLK_z:
                    buttonZ = true;
                    render_wireframe = true;
                    break;
                case SDLK_F1:
                    buttonF1 = true;
                    screenshotHandler.takeScreenshot();
                    break;
                case SDLK_1:
                    player->set_inventory_slot(1);
                    if (buttonShift){debug_render_state=0;}
                    break;
                case SDLK_2:
                    player->set_inventory_slot(2);
                    if (buttonShift){debug_render_state=1;}
                    break;
                case SDLK_3:
                    player->set_inventory_slot(3);
                    if (buttonShift){debug_render_state=2;}
                    break;
                case SDLK_4:
                    player->set_inventory_slot(4);
                    if (buttonShift){debug_render_state=3;}
                    break;
                case SDLK_5:
                    player->set_inventory_slot(5);
                    if (buttonShift){debug_render_state=4;}
                    break;
                case SDLK_6:
                    player->set_inventory_slot(6);
                    if (buttonShift){debug_render_state=5;}
                    break;
                case SDLK_7:
                    player->set_inventory_slot(7);
                    break;
                case SDLK_8:
                    player->set_inventory_slot(8);
                    break;
                case SDLK_9:
                    player->set_inventory_slot(9);
                    break;
            }
        }
        else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
                case SDLK_w:
                    buttonW = false;
                    break;
                case SDLK_a:
                    buttonA = false;
                    break;
                case SDLK_s:
                    buttonS = false;
                    break;
                case SDLK_d:
                    buttonD = false;
                    break;
                case SDLK_SPACE:
                    buttonSpace = false;
                    releasedSpaceSinceJump = true;
                    break;
                case SDLK_LSHIFT:
                    buttonShift = false;
                    break;
                case SDLK_LCTRL:
                    buttonCtrl = false;
                    break;
                case SDLK_z:
                    buttonZ = false;
                    render_wireframe = false;
                    break;
                case SDLK_c:
                    buttonC = false;
                    break;
                case SDLK_F1:
                    buttonF1 = false;
                    break;
            }
        }
        else if (event.type == SDL_MOUSEMOTION) {
            player->onMouseMoved(event.motion.xrel, event.motion.yrel);
            player->update_selection_box();
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouseButtonL = true;
                mouseButtonClickL = true;
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                mouseButtonR = true;
                mouseButtonClickR = true;
            }

        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouseButtonL = false;
            }
            if (event.button.button == SDL_BUTTON_RIGHT) {
                mouseButtonR = false;
            }

        }
    }
}

void GameInstance::applyGameMechanics() {
    glClearColor(0.5f, 0.5f, 0.8f, 1.0f); // set Color to zero again and use a cubemap background

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    time += delta;
    ticks_since_last_jump++;

    player->move(delta);

    if (buttonW) {
        player->moveFront(delta);
    }if (buttonA) {
        player->moveSideways(-delta);
    }if (buttonS) {
        player->moveFront(-delta);
    }if (buttonD) {
        player->moveSideways(delta);
    }if (buttonSpace) {
        if (ticks_since_last_jump < 30 and releasedSpaceSinceJump){
            player->jump(true, delta);
        }else{
            player->jump(false, delta);
            ticks_since_last_jump = 0;
        }
        releasedSpaceSinceJump = false;
    }
    if (buttonShift) {
        player->moveUp(-delta);
    }
    if (buttonCtrl) {
        player->setIsSprinting(true);
    }else{
        player->setIsSprinting(false);
    }
    if(player->getMovementState()==Walking){
        player->apply_gravity(delta, glm::vec3(0.0f, -0.5f, 0.0f));
    }
    if (mouseButtonL) {
        if (time_since_left_tick > 0.2){
            time_since_left_tick = 0;
            player->get_focussed_Block();
        }
    }
    if (mouseButtonR) {
        if (time_since_right_tick > 0.2){
            time_since_right_tick = 0;
            player->place_block();
        }

    }
    player->zoomIn(buttonC);

    player->update();

    ui->setSlot(player->get_inventory_slot()-1);
    ItemFont->clear();
    player->Update_Inventory_Items(ui, ItemFont);


    std::stringstream ss;
    string output;
    ss << "\n";
    ss << vec3_toString(player->getCurrentChunkPos(), "ChunkPos: ") << std::endl;
    ss << vec3_toString(player->getPosition(), "pos: ") << std::endl;
    ss << vec3_toString(player->getCamera()->getPos(), "CAMERA pos: ") << std::endl;
    ss << "Number of Chunks: "<< SuperChunk::getNumChunks() << std::endl;
    ss << vec3_toString(position_in_chunk(player->getPosition()), "Player pos in chunk") << std::endl;
    output = ss.str();

    if (player->hasChangedChunk()) {
        std::cout << "Player has moved to another Chunk" << std::endl;
        SuperChunk::updateChunkLoadingData(player->getLocalChunkIds());
    }

    SuperChunk::load_unload_singleChunk();
    SuperChunk::load_unload_singleChunk();
    SuperChunk::load_unload_singleChunk();

    SuperChunk::load_unload_singleChunk();
    SuperChunk::load_unload_singleChunk();
    SuperChunk::load_unload_singleChunk();

    SuperChunk::load_unload_singleChunk();
    SuperChunk::load_unload_singleChunk();
    SuperChunk::load_unload_singleChunk();

    SuperChunk::getGeneratedChunks();
}

void GameInstance::render() {
    renderer->clear();

    // First rendering stage: solid surfaces
    renderer->setModeSolid();

    // rendering the background
    player->render_skybox();

    if (render_wireframe){
        GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }

    // rendering all opaque blocks
    SuperChunk::render(player->getModelViewProj_GL());

    renderer->setModeSemiSolid();
    SuperChunk::render_foliage(player->getModelViewProj_GL());

    // rendering the players selection box ontop of the blocks
    player->render_selection_box();



    // Second rendering stage: transparent surfaces
    renderer->setModeTransparent();

    // rendering all transparent blocks
    SuperChunk::render_transparent(player->getModelViewProj_GL());

//    if (graphicalDebug){
//        SuperChunk::renderDebug_Box(player->getSelectionPos(), player->getModelViewProj_GL());
//    }

    GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

    renderer->setModeGui();

    UIShader->bind();
    ui->render(UItexture->get_textureId(), texture->get_textureId());

    fontShader->bind();
    font->update();
    glm::vec3 color = {1.0, 1.0, 1.0};
    glUniform3f(glGetUniformLocation(fontShader->getShaderId(), "u_textColor"), color.x, color.y, color.z);
    font->render();

    ItemFont->update();
    ItemFont->render();

    // Third rendering stage: Compositing
    renderer->setModeComposite();
    compositeShader->bind();
    // rendering the composed image
    renderer->drawScreen();

    // Final stage: Showing the result on the screen
    renderer->setModeScreen();
    screenShader->bind();

    glUniform1i(glGetUniformLocation(screenShader->getShaderId(), "debug_render_state"), debug_render_state);
    renderer->drawScreen();



    SDL_handler::SDL_SwapWindow();


    // Todo: include in SDL_handler
    uint64 endCounter = SDL_GetPerformanceCounter();
    uint64 counterElapsed = endCounter - lastCounter;
    delta = ((float32)counterElapsed) / (float32)perfCounterFrequency;
    uint32 FPS = (uint32)((float32)perfCounterFrequency / (float32)counterElapsed);
    time_since_slow_tick += delta;
    time_since_left_tick += delta;
    time_since_right_tick += delta;
    std::stringstream ss;
    ss.str(std::string());
    if (time_since_slow_tick > 1.0){
        int x,y,z;
        calcChunkCoords(player->getPosition(),&x, &y, &z);
        ss << "FPS: " << FPS << std::endl;
        ss << "Number of Chunks: "<< SuperChunk::getNumChunks() << ",  " << SuperChunk::getNumFilledChunks() << std::endl;
        ss << vec3_toString(player->getPosition(), "pos ") << std::endl;
        //ss << vec3_toString({x,y,z}, "pos ") << std::endl;getVelocity
        ss << vec3_toString(player->getVelocity(), "vel ") << std::endl;
        ss << "Number of Faces: " << SuperChunk::get_num_all_faces() << std::endl;
        ss << "Sight distance: " << player->get_sight_distance() << std::endl;

        ss << "ticks_since_last_jump: " << ticks_since_last_jump<< std::endl;
//        ss << player->getJumpMethod() << std::endl;
//        ss << player->getMovementState() << std::endl;
        text = ss.str();
        time_since_slow_tick = 0;

        font->clear();
        font->addMultipleLines(text, -0.95, 0.9, 0.03);
    }
    lastCounter = endCounter;

    mouseButtonClickL = false;
    mouseButtonClickR = false;
    //buttonSpacePress = false;
}

void GameInstance::runGameLoop() {
    while (!close) {
        handleInput();
        applyGameMechanics();
        render();
    }
    stop();
}

