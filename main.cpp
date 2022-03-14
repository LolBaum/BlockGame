#include <iostream>
#include <cmath>
#define GLEW_STATIC
#include <glew.h>
#define SDL_MAIN_HANDLED // remove this line
#include <SDL.h>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//only for Windows
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")



#include <glm/gtc/noise.hpp>



#include "definitions.hpp"
#include "Vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "shader.hpp"
#include "object_models.hpp"
#include "floating_camera.hpp"

#include "printFunktions.hpp"
#include "util_funcs.hpp"
#include "text.hpp"
#include "screenshot.hpp"

#include "textures.hpp"

#include "local_player_data.hpp"

#include "Block.hpp"
#include "Chunk.hpp"
#include "SuperChunk.hpp"

#include "SDL_handler.hpp"
//#include <filesystem>
#include <sstream>



//namespace fs = std::filesystem; // Needs C++ 17 or above 


// -------------------------------------------------------
// WIP

//FontSystem *Fs;



// Main Program





int main_function() {
	btm.AddBlockType(BlockType(1, "Stone", SingleTexture, 0, 1));
	btm.AddBlockType(BlockType(2, "Dirt", SingleTexture, 1, 0));
	btm.AddBlockType(BlockType(3, SpecialBlockTexture(2,0, 2,0, 2,0, 2,0, 3,0, 1,0), "Grass", MultiTexture));
	btm.AddBlockType(BlockType(4, "Cobblestone", SingleTexture, 1, 1));
	btm.AddBlockType(BlockType(5, SpecialBlockTexture(0,2, 0,2, 0,2, 0,2, 1,2, 1,2), "Wood Log", MultiTexture));
	btm.AddBlockType(BlockType(6, "Wooden Planks", SingleTexture, 0, 3));
	btm.AddBlockType(BlockType(7, "Leafs", SingleTexture, 2, 3, false));
	//BlockType b = *btm.GetBlockType(0);
	//b.printInfo();
	
	sdl_handler.initialize();

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		std::cout << "Press any Key to exit." << std::endl;
		std::cin.get();
		return -1;
	}
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;


	//        Wichtig
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(openGLDebugCallback, 0);
#endif

	FontSystem Fontsys = FontSystem();
	//Fs = &Fontsys;
	Fontsys.initialize();
	Font font0 = Fontsys.create_Font("fonts/TheJewishBitmap.ttf");
	Font font = Fontsys.create_Font("fonts/BLKCHCRY.TTF");

	chunkManager = SuperChunk();
	chunkManager.initialize();



	LocalPlayer player = LocalPlayer();

	//using Stacks for vertical World Inforamtion e.g. height...

/* 	int32 textureWidth = 0;
	int32 textureHeight = 0;
	int32 bitsPerPixel = 0;
	stbi_set_flip_vertically_on_load(true);
	//auto textureBuffer = stbi_load("Graphics/gras_test.png", &textureWidth, &textureHeight, &bitsPerPixel, 4);
	//auto textureBuffer = stbi_load("Graphics/hammerMC.png", &textureWidth, &textureHeight, &bitsPerPixel, 4);
	//auto textureBuffer = stbi_load("Graphics/HA_logo_saved.jpg", &textureWidth, &textureHeight, &bitsPerPixel, 4);
	//auto textureBuffer = stbi_load("Graphics/uvGrid.png", &textureWidth, &textureHeight, &bitsPerPixel, 4);
	auto textureBuffer = stbi_load("Graphics/TileAtlas64.png", &textureWidth, &textureHeight, &bitsPerPixel, 4);

	GLuint textureId;
	GLCALL(glGenTextures(1, &textureId));
	GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
	GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	//GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
	

	if (textureBuffer) {
		stbi_image_free(textureBuffer);
	} */

	


	Shader shader = chunkManager.get_shader();//shader("shaders/basic.vs", "shaders/basic.fs");
	Texture texture = chunkManager.get_tile_atlas();
	shader.bind();

	uint64 perfCounterFrequency = SDL_GetPerformanceFrequency();
	uint64 lastCounter = SDL_GetPerformanceCounter();
	float32 delta = 0.0f;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_MULTISAMPLE);
	//glDepthFunc(GL_ALWAYS);

	//glEnable(GL_NORMALIZE);

	/* int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
	if (colorUniformLocation != -1) {
		GLCALL(glUniform4f(colorUniformLocation, 1.0f, 0.0f, 1.0f, 1.0f));
	}

	int TextureUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_texture");
	if (TextureUniformLocation != -1) {
		GLCALL(glUniform1i(TextureUniformLocation, 0));
	}

	int modelViewProjMatrixLocation = GLCALL(glGetUniformLocation(shader.getShaderId(), "u_modelViewProj")); */






	// Wireframe
	//GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	// Faces
	//GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	std::string text = "FPS: ";
	float time_since_slow_tick = 0;
	float time_since_left_tick = 0;
	float time_since_right_tick = 0;

	bool buttonW = false;
	bool buttonA = false;
	bool buttonS = false;
	bool buttonD = false;
	bool buttonSpace = false;
	//bool buttonSpacePress = false;
	bool buttonShift = false;
	bool buttonZ = false;
	bool buttonF1 = false;
	bool mouseButtonL = false;
	bool mouseButtonR = false;
	bool mouseButtonClickL = false;
	bool mouseButtonClickR = false;

	bool close = false;
	sdl_handler.SetRelativeMouseMode(true); // in Init ?
	//float cameraSpeed = 6.0f;
	float time = 0.0f;
	while (!close) {

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
				case SDLK_SPACE:
					buttonSpace = true;
					//buttonSpacePress = true;
					//std::cout << "pressed Space" << std::endl;
					break;
				case SDLK_LSHIFT:
					buttonShift = true;
					break;
				case SDLK_z:
					buttonZ = true;
					GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
					break;
				case SDLK_F1:
					buttonF1 = true;
					saveScreenshotToFile(numerate_name("screenshots/screenshot_", ".png"), sdl_handler.getWidth(), sdl_handler.getHeight());
					break;
				case SDLK_1:
					player.set_selected_block_type(1);
					break;
				case SDLK_2:
					player.set_selected_block_type(2);
					break;
				case SDLK_3:
					player.set_selected_block_type(3);
					break;
				case SDLK_4:
					player.set_selected_block_type(4);
					break;
				case SDLK_5:
					player.set_selected_block_type(5);
					break;
				case SDLK_6:
					player.set_selected_block_type(6);
					break;
				case SDLK_7:
					player.set_selected_block_type(7);
					break;
				case SDLK_8:
					player.set_selected_block_type(8);
					break;
				case SDLK_9:
					player.set_selected_block_type(9);
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
					break;
				case SDLK_LSHIFT:
					buttonShift = false;
					break;
				case SDLK_z:
					buttonZ = false;
					GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
					break;
				case SDLK_F1:
					buttonF1 = false;
					break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION) {
				player.getCamera()->onMouseMoved(event.motion.xrel, event.motion.yrel);
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
		glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		time += delta;

		/*
		if (colorUniformLocation != -1) {
			GLCALL(glUniform4f(colorUniformLocation, sinf(time), cosf(time) * cosf(time), 1.0f, 1.0f));
		}
		*/

		/* if (buttonW) {
			player.getCamera()->moveFront(delta * cameraSpeed);
		}if (buttonA) {
			player.getCamera()->moveSideways(-delta * cameraSpeed);
		}if (buttonS) {
			player.getCamera()->moveFront(-delta * cameraSpeed);
		}if (buttonD) {
			player.getCamera()->moveSideways(delta * cameraSpeed);
		}if (buttonSpace) {
			player.getCamera()->moveUp(delta * cameraSpeed);
		}if (buttonShift) {
			player.getCamera()->moveUp(-delta * cameraSpeed);
		} */

		player.move(delta);

		if (buttonW) {
			player.moveFront(delta);
		}if (buttonA) {
			player.moveSideways(-delta);
		}if (buttonS) {
			player.moveFront(-delta);
		}if (buttonD) {
			player.moveSideways(delta);
		}if (buttonSpace) {
			player.jump();
			//player.moveUp(delta);
		}
		if (buttonShift) {
			player.moveUp(-delta);
		}
		player.apply_gravity(delta, glm::vec3(0.0f, -0.5f, 0.0f));
		//player.add_force(delta, glm::vec3(0.0f, -0.5f, 0.0f));
		
		
		if (mouseButtonL) {
			if (time_since_left_tick > 0.2){
				time_since_left_tick = 0;
				player.get_focussed_Block();
			}
		}
		if (mouseButtonR) {
			if (time_since_right_tick > 0.2){
				time_since_right_tick = 0;
				player.place_block();
			}
			
		}
		
/* 		if (mouseButtonClickL) {
			player.get_focussed_Block();
		}
		if (mouseButtonClickR) {
			player.place_block();
		} */

		

		player.update();


		std::stringstream ss;
		string output;
		ss << "\n";
		ss << vec3_toString(player.getCurrentChunkPos(), "ChunkPos: ") << std::endl;
		ss << vec3_toString(player.getPosition(), "pos: ") << std::endl;
		ss << vec3_toString(player.getCamera()->getPos(), "CAMERA pos: ") << std::endl;
		ss << "Number of Chunks: "<< chunkManager.getNumChunks() << std::endl;
		ss << vec3_toString(position_in_chunk(player.getPosition()), "Player pos in chunk") << std::endl;
		output = ss.str();
		slowPrint(output);

		if (player.hasChangedChunk()) {
			std::cout << "Player has moved to another Chunk" << std::endl;
			//player.printLocalChunks();
			chunkManager.updateChunkLoadingData(player.getLocalChunkIds());
			//chunkManager.updateChunkLoadingData(player.getLocalChunkIds());
			//chunkManager.updateChunkLoadingData(player.getLocalChunkIds());


			//chunkManager.loadChunks(player.getLocalChunkIds());
		}
		chunkManager.load_unload_singleChunk();
		chunkManager.load_unload_singleChunk();
		chunkManager.load_unload_singleChunk();

		chunkManager.load_unload_singleChunk();
		chunkManager.load_unload_singleChunk();
		chunkManager.load_unload_singleChunk();

		chunkManager.load_unload_singleChunk();
		chunkManager.load_unload_singleChunk();
		chunkManager.load_unload_singleChunk();
		

		chunkManager.render(player.getModelViewProj_GL());


		Fontsys.render_multiline_text(text, font, 25, sdl_handler.getHeight()-50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		//Fontsys.RenderText("Hello World :)", font0, 25, sdl_handler.getHeight()-200, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));


		sdl_handler.SDL_SwapWindow();


		// Todo: include in SDL_handler
		uint64 endCounter = SDL_GetPerformanceCounter();
		uint64 counterElapsed = endCounter - lastCounter;
		delta = ((float32)counterElapsed) / (float32)perfCounterFrequency;
		uint32 FPS = (uint32)((float32)perfCounterFrequency / (float32)counterElapsed);
		time_since_slow_tick += delta;
		time_since_left_tick += delta;
		time_since_right_tick += delta;
		ss.str(std::string());
		if (time_since_slow_tick > 1.0){
			ss << "FPS: " << FPS << std::endl;
			ss << "Number of Chunks: "<< chunkManager.getNumChunks() << ",  " << chunkManager.getNumFilledChunks() << std::endl;
			ss << vec3_toString(player.getPosition(), "pos ") << std::endl;
			ss << vec3_toString(player.getCamera()->getViewPos(), "cam ") << std::endl;
			ss << btm.GetBlockType(player.get_selected_block_type())->info_srting() << std::endl;
			ss << "Number of Faces: " << chunkManager.get_num_all_faces() << std::endl;
			text = ss.str();
			time_since_slow_tick = 0;
		}
		lastCounter = endCounter;

		mouseButtonClickL = false;
		mouseButtonClickR = false;
		//buttonSpacePress = false;
	}

	GLCALL(glDeleteTextures(1, texture.get_textureId_ptr()));

	return 0;
}

int main(int argc, char** argv) {
	std::cout << "Starting Program" << std::endl;
	int r;
	r = main_function();

	std::cout << "Program exits" << std::endl;
	return r;
}