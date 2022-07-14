#include "main.hpp"

#include <iostream>
#include <cmath>
#define GLEW_STATIC
#include <glew.h>
#define SDL_MAIN_HANDLED // remove this line
#include <SDL.h>

#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"


//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#define STB_IMAGE_STATIC
//#include "core_includes.hpp"

//only for Windows
#ifdef _WIN32
	#pragma comment(lib, "SDL2.lib")
	#pragma comment(lib, "glew32s.lib")
	#pragma comment(lib, "opengl32.lib")
#else

#endif


#include <glm/gtc/noise.hpp>



#include "definitions.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "frame_buffer.hpp"
#include "renderer.hpp"
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

#include "item.hpp"

#include "SDL_handler.hpp"

#include "config.hpp"
//#include <filesystem>
#include <sstream>



//namespace fs = std::filesystem; // Needs C++ 17 or above 


// -------------------------------------------------------
// WIP

//FontSystem *Fs;




// Main Program





int main_function() {
	BlockTypeManager::AddBlockType(BlockType(1, "Stone", SingleTexture, 0, 1));
	BlockTypeManager::AddBlockType(BlockType(2, "Dirt", SingleTexture, 1, 0));
	BlockTypeManager::AddBlockType(BlockType(3, SpecialBlockTexture(2,0, 2,0, 2,0, 2,0, 3,0, 1,0), "Grass", MultiTexture));
	BlockTypeManager::AddBlockType(BlockType(4, "Cobblestone", SingleTexture, 1, 1));
	BlockTypeManager::AddBlockType(BlockType(5, SpecialBlockTexture(0,2, 0,2, 0,2, 0,2, 1,2, 1,2), "Wood Log", MultiTexture));
	BlockTypeManager::AddBlockType(BlockType(6, "Wooden Planks", SingleTexture, 0, 3));
	//BlockTypeManager::AddBlockType(BlockType(6, "Withe Test - no collision", SingleTexture, 3, 1, Solid, false));
	BlockTypeManager::AddBlockType(BlockType(7, "Leafs", SingleTexture, 2, 3, Transparent_opaque));
	BlockTypeManager::AddBlockType(BlockType(8, "Glass", SingleTexture, 3, 2, Transparent_opaque));
	BlockTypeManager::AddBlockType(BlockType(9, "Ice", SingleTexture, 3, 3, Transparent));


	ItemTypeManager::AddItemType(new ItemType(10, "Stick"));

	Config::init();
	//Config::saveConfig();



	//BlockType b = *BlockTypeManager::GetBlockType(0);
	//b.printInfo();
	
	SDL_handler::initialize();

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

	Font font = Font();
	Font ItemFont = Font();
	//FontSystem Fontsys = FontSystem();
	//Fs = &Fontsys;
	//Fontsys.initialize();
	//Font font0 = Fontsys.create_Font("fonts/TheJewishBitmap.ttf");
	//Font font = Fontsys.create_Font("fonts/BLKCHCRY.TTF");

	World::init();


	SuperChunk::initialize();



	LocalPlayer player = LocalPlayer();

	Shader shader = SuperChunk::get_shader();//shader("shaders/basic.vs", "shaders/basic.fs");
	Texture texture = *SuperChunk::get_tile_atlas();
	shader.bind();

	uint64 perfCounterFrequency = SDL_GetPerformanceFrequency();
	uint64 lastCounter = SDL_GetPerformanceCounter();
	float32 delta = 0.0f;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_MULTISAMPLE);
	//glDepthFunc(GL_ALWAYS);

	//glEnable(GL_NORMALIZE);

	Shader compositeShader("shaders/composite.vs", "shaders/composite.fs");
	Shader screenShader("shaders/frame.vs", "shaders/frame.fs");

	Renderer renderer = Renderer();

	Shader UIShader("shaders/UI.vs", "shaders/UI.fs");
	Texture UItexture = Texture("graphics/UI.png");
	
	//UImesh ui = ItemBarMesh();
	InventoryMesh ui = InventoryMesh();




	/* glm::vec3 ptest = glm::vec3(0.1, 0.1, 0.1);


	ui.addQuad(ptest);*/
	ui.update(); 


	Shader fontShader("shaders/font.vs", "shaders/font.fs");



	//Shader skySahder("shaders/sky.vs", "shaders/sky.fs");







	// Wireframe
	//GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	// Faces
	//GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	std::string text = "FPS: ";
	float time_since_slow_tick = 1.0;
	float time_since_left_tick = 0;
	float time_since_right_tick = 0;

	bool show_text = true;

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
	SDL_handler::SetRelativeMouseMode(true); // in Init ?
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
				case SDLK_y:
					show_text = !show_text;
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
					saveScreenshotToFile(numerate_name("screenshots/screenshot_", ".png"), SDL_handler::getWidth(), SDL_handler::getHeight());
					break;
				case SDLK_1:
					player.set_inventory_slot(1);
					break;
				case SDLK_2:
					player.set_inventory_slot(2);
					break;
				case SDLK_3:
					player.set_inventory_slot(3);
					break;
				case SDLK_4:
					player.set_inventory_slot(4);
					break;
				case SDLK_5:
					player.set_inventory_slot(5);
					break;
				case SDLK_6:
					player.set_inventory_slot(6);
					break;
				case SDLK_7:
					player.set_inventory_slot(7);
					break;
				case SDLK_8:
					player.set_inventory_slot(8);
					break;
				case SDLK_9:
					player.set_inventory_slot(9);
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
				player.update_selection_box();
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
		glClearColor(0.5f, 0.5f, 0.8f, 1.0f); // set Color to zero again and use a cubemap background
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

		ui.setSlot(player.get_inventory_slot()-1);
		ItemFont.clear();
		Update_Inventory_Items(player.get_inventory(), &ui, &ItemFont);


		std::stringstream ss;
		string output;
		ss << "\n";
		ss << vec3_toString(player.getCurrentChunkPos(), "ChunkPos: ") << std::endl;
		ss << vec3_toString(player.getPosition(), "pos: ") << std::endl;
		ss << vec3_toString(player.getCamera()->getPos(), "CAMERA pos: ") << std::endl;
		ss << "Number of Chunks: "<< SuperChunk::getNumChunks() << std::endl;
		ss << vec3_toString(position_in_chunk(player.getPosition()), "Player pos in chunk") << std::endl;
		output = ss.str();
		//slowPrint(output);

		if (player.hasChangedChunk()) {
			std::cout << "Player has moved to another Chunk" << std::endl;
			//player.printLocalChunks();
			SuperChunk::updateChunkLoadingData(player.getLocalChunkIds());
			//SuperChunk::updateChunkLoadingData(player.getLocalChunkIds());
			//SuperChunk::updateChunkLoadingData(player.getLocalChunkIds());


			//SuperChunk::loadChunks(player.getLocalChunkIds());
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

		renderer.clear();


		// First rendering stage: solid surfaces
		renderer.setModeSolid();


		
		// rendering the background
		//skySahder.bind();
		player.render_skybox();


		// rendering all opaque blocks
		SuperChunk::render(player.getModelViewProj_GL());

		// rendering the players selection box ontop of the blocks
		player.render_selection_box(); 	


		
		
		

		// Second rendering stage: transparent surfaces
		renderer.setModeTransparent();
		// rendering all transparent blocks
		SuperChunk::render_transparent(player.getModelViewProj_GL());
																

		// TEMP: All text needs to be rendered again with the transparent FB so the transparent blocks are not showing infornt of it
		// Idea: adding a FrameBuffer to handle drawing to all involved textures at once
		//		 or adding another layer to the Compositing for Text.
		//renderer.setModeText();

		// if (show_text){
		// 	Fontsys.render_multiline_text(text, font, 25, SDL_handler::getHeight()-50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f)); 
		// } 


		//Fontsys.render_multiline_text(text, font, 25, SDL_handler::getHeight()-50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f)); ///////////////////
		//Fontsys.RenderText("Hello World :)", font0, 25, SDL_handler::getHeight()-200, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));


		renderer.setModeGui();	

		UIShader.bind();
		ui.render(UItexture.get_textureId(), texture.get_textureId());
		// rendering the info text
		// if (show_text){
		// 	Fontsys.render_multiline_text(text, font, 25, SDL_handler::getHeight()-50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f)); 
		// }

		fontShader.bind();
		font.update();
		glm::vec3 color = {1.0, 1.0, 1.0};
		glUniform3f(glGetUniformLocation(fontShader.getShaderId(), "u_textColor"), color.x, color.y, color.z);
		font.render();

		ItemFont.update();
		ItemFont.render();


		// Third rendering stage: Compositing
		renderer.setModeComposite();
		compositeShader.bind();
		// rendering the composed image
		renderer.drawScreen();

		// Final stage: Showing the result on the screen
		renderer.setModeScreen();
		screenShader.bind();
		renderer.drawScreen();


		
		SDL_handler::SDL_SwapWindow();


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
			ss << "Number of Chunks: "<< SuperChunk::getNumChunks() << ",  " << SuperChunk::getNumFilledChunks() << std::endl;
			ss << vec3_toString(player.getPosition(), "pos ") << std::endl;
			//ss << vec3_toString(player.getCamera()->getViewPos(), "cam ") << std::endl;
			//ss << ItemTypeManager::GetItemType(player.get_selected_item_type())->info_string() << std::endl;
			ss << "Number of Faces: " << SuperChunk::get_num_all_faces() << std::endl;
			ss << "Sight distance: " << player.get_sight_distance() << std::endl;
			//glm::vec3 dummy_cam_rotation = player.getCamera()->getRotation();
			//ss << "Yaw: " << dummy_cam_rotation.x << std::endl;
			//ss << "Pitch: " << dummy_cam_rotation.y << std::endl;
			//ss << player.inventory_as_string() << std::endl;
			//ss << "can this (╯°□°)╯︵ ┻━┻ be rendered?" << std::endl; // no currently it wont be displayed
			//player.debug_print_inventory();
			text = ss.str();
			time_since_slow_tick = 0;
			
			font.clear();
			//font.addLine("Hello World :)\024", -0.5, 0.0, 0.1);
			font.addMultipleLines(text, -0.95, 0.9, 0.05);
		}
		lastCounter = endCounter;

		mouseButtonClickL = false;
		mouseButtonClickR = false;
		//buttonSpacePress = false;
	}
	SuperChunk::saveWorld();

	GLCALL(glDeleteTextures(1, texture.get_textureId_ptr()));


	player.setPlayerValues_inWorldData();

	World::saveWorldFile();
	Config::saveConfig();

	return 0;
}

int main(int argc, char** argv) {
	std::cout << "Starting Program" << std::endl;
	int r;
	r = main_function();

	std::cout << "Program exits" << std::endl;
	return r;
}