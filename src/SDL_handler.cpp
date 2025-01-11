#include "SDL_handler.hpp"

#include "config.hpp"

SDL_handler::SDL_handler(){}
SDL_handler::~SDL_handler() {}

SDL_Window* SDL_handler::window;
int SDL_handler::width;
int SDL_handler::height;


void SDL_handler::SetWindowSize_ToDisplay(){
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    width = DM.w;
    height = DM.h;
}


void SDL_handler::initialize() {
    std::cout << "Initializing the SDL" << std::endl;

    width = Config::getIntValue("WindowX"); //1440;
    height = Config::getIntValue("WindowY"); //900;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);


    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    /* SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); 
*/

    SDL_GL_SetSwapInterval(1); // Vsync 

#ifdef _DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    uint32 flags = SDL_WINDOW_OPENGL;
    if (Config::getBoolValue("Fullscreen")){
        flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
        SetWindowSize_ToDisplay();
    }

    if (Config::getBoolValue("WindowMaximezed")){
        flags = flags | SDL_WINDOW_BORDERLESS;
        SetWindowSize_ToDisplay();
    }

    // if (Config::getBoolValue("WindowBorderless")){
    //     flags = flags | SDL_WINDOW_BORDERLESS;
    //     SetWindowSize_ToDisplay();
    // }

    

    std::stringstream Window_title;
    Window_title << GAME_NAME << "   version: " << GAME_VERSION_MAJOR << "." << GAME_VERSION_MINOR << std::endl;

    window = SDL_CreateWindow(Window_title.str().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
}

void SDL_handler::SDL_SwapWindow() {
    SDL_GL_SwapWindow(window);
}

void SDL_handler::SetRelativeMouseMode(bool b) {
    if (b) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    else {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}

int SDL_handler::getWidth() {
    return width;
}
int SDL_handler::getHeight() {
    return height;
}
float SDL_handler::getAspectRatio() {
    return (float)height/(float)width;
}
SDL_Window* SDL_handler::getWindow(){
    return window;
}

