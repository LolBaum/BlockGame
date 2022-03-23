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

#include "SDL_handler.hpp"
//#include <filesystem>
#include <sstream>



//namespace fs = std::filesystem; // Needs C++ 17 or above 


// -------------------------------------------------------
// WIP

//FontSystem *Fs;
glm::mat4 calculate_model_matrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, position);
	trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, scale);

	return trans;
}



// Main Program





int main_function() {
	btm.AddBlockType(BlockType(1, "Stone", SingleTexture, 0, 1));
	btm.AddBlockType(BlockType(2, "Dirt", SingleTexture, 1, 0));
	btm.AddBlockType(BlockType(3, SpecialBlockTexture(2,0, 2,0, 2,0, 2,0, 3,0, 1,0), "Grass", MultiTexture));
	btm.AddBlockType(BlockType(4, "Cobblestone", SingleTexture, 1, 1));
	btm.AddBlockType(BlockType(5, SpecialBlockTexture(0,2, 0,2, 0,2, 0,2, 1,2, 1,2), "Wood Log", MultiTexture));
	//btm.AddBlockType(BlockType(6, "Wooden Planks", SingleTexture, 0, 3));
	btm.AddBlockType(BlockType(6, "Withe Test - no collision", SingleTexture, 3, 1, Solid, false));
	btm.AddBlockType(BlockType(7, "Leafs", SingleTexture, 2, 3, Transparent_opaque));
	btm.AddBlockType(BlockType(8, "Glass", SingleTexture, 3, 2, Transparent_opaque));
	btm.AddBlockType(BlockType(9, "Ice", SingleTexture, 3, 3, Transparent));
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



	// WIP Framebuffer
	// from: https://learnopengl.com/Advanced-OpenGL/Framebuffers
	
	/* unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); 
	
	// generate texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1440, 900, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);  

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1440, 900);  
	glBindRenderbuffer(GL_RENDERBUFFER, 0);


	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);   */


	
	//Renderer renderer = Renderer();
	//FrameBuffer* frame_buffer = renderer.get_fb();


	//ScreenQuad quad = ScreenQuad();

	//Vertex2DBuffer quad_vertex_buffer = Vertex2DBuffer(quad.getVertices(), quad.getNumVertices());
	//IndexBuffer quad_index_buffer = IndexBuffer(quad.getIndices(), quad.getNumIndices(), sizeof(quad.getIndices()[0]));


	// float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    //     // positions   // texCoords
    //     -1.0f,  1.0f,  0.0f, 1.0f,
    //     -1.0f, -1.0f,  0.0f, 0.0f,
    //      1.0f, -1.0f,  1.0f, 0.0f,

    //     -1.0f,  1.0f,  0.0f, 1.0f,
    //      1.0f, -1.0f,  1.0f, 0.0f,
    //      1.0f,  1.0f,  1.0f, 1.0f
    // };

	// unsigned int quadVAO, quadVBO;
    // glGenVertexArrays(1, &quadVAO);
    // glGenBuffers(1, &quadVBO);
    // glBindVertexArray(quadVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	//Shader screenShader = Shader("shaders/frame.vs", "shaders/frame.fs");



	//Shader solidShader("solid.vs", "solid.fs");
	//Shader transparentShader("transparent.vs", "transparent.fs");
	Shader compositeShader("shaders/composite.vs", "shaders/composite.fs");
	Shader screenShader("shaders/frame.vs", "shaders/frame.fs");

	int SCR_WIDTH = sdl_handler.getWidth();
	int SCR_HEIGHT = sdl_handler.getHeight();


	float quadVertices[] = {
		// positions		// uv
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,

		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	// set up framebuffers and their texture attachments
	// ------------------------------------------------------------------
	unsigned int opaqueFBO, transparentFBO;
	glGenFramebuffers(1, &opaqueFBO);
	glGenFramebuffers(1, &transparentFBO);

	// set up attachments for opaque framebuffer
	unsigned int opaqueTexture;
	glGenTextures(1, &opaqueTexture);
	glBindTexture(GL_TEXTURE_2D, opaqueTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, opaqueTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Opaque framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// set up attachments for transparent framebuffer
	unsigned int accumTexture;
	glGenTextures(1, &accumTexture);
	glBindTexture(GL_TEXTURE_2D, accumTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int revealTexture;
	glGenTextures(1, &revealTexture);
	glBindTexture(GL_TEXTURE_2D, revealTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, revealTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0); // opaque framebuffer's depth texture

	const GLenum transparentDrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, transparentDrawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Transparent framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// set up transformation matrices
	// ------------------------------------------------------------------
/* 	glm::mat4 redModelMat = calculate_model_matrix(glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 greenModelMat = calculate_model_matrix(glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 blueModelMat = calculate_model_matrix(glm::vec3(0.0f, 0.0f, 2.0f)); */

	// set up intermediate variables
	// ------------------------------------------------------------------
	glm::vec4 zeroFillerVec(0.0f);
	glm::vec4 oneFillerVec(1.0f);




	// Wireframe
	//GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	// Faces
	//GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	std::string text = "FPS: ";
	float time_since_slow_tick = 0;
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


		std::stringstream ss;
		string output;
		ss << "\n";
		ss << vec3_toString(player.getCurrentChunkPos(), "ChunkPos: ") << std::endl;
		ss << vec3_toString(player.getPosition(), "pos: ") << std::endl;
		ss << vec3_toString(player.getCamera()->getPos(), "CAMERA pos: ") << std::endl;
		ss << "Number of Chunks: "<< chunkManager.getNumChunks() << std::endl;
		ss << vec3_toString(position_in_chunk(player.getPosition()), "Player pos in chunk") << std::endl;
		output = ss.str();
		//slowPrint(output);

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


		// first pass
		// glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
		// glEnable(GL_DEPTH_TEST);


		//frame_buffer->bind();
/* 		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.render_to_frame_buffer(); */

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glClearColor(0.6, 0.7, 1.0, 1.0); // set Color to zero again and use a cubemap background

		// bind opaque framebuffer to render solid objects
		glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		chunkManager.render(player.getModelViewProj_GL());
		if (show_text){
			Fontsys.render_multiline_text(text, font, 25, sdl_handler.getHeight()-50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f)); 
		}
		
/* 
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunci(0, GL_ONE, GL_ONE);
		glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
		glBlendEquation(GL_FUNC_ADD);
		glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
		glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);
		renderer.render_to_frame_buffer_transparent(); */

		// configure render states
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunci(0, GL_ONE, GL_ONE);
		glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
		glBlendEquation(GL_FUNC_ADD);

		// bind transparent framebuffer to render transparent objects
		glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
		glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
		glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);
		chunkManager.render_transparent(player.getModelViewProj_GL());

/* 
		renderer.render_to_frame_buffer();*/
		shader.bind();
		player.render_selection_box(); 																				/////////////////


		//Fontsys.render_multiline_text(text, font, 25, sdl_handler.getHeight()-50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f)); ///////////////////
		//Fontsys.RenderText("Hello World :)", font0, 25, sdl_handler.getHeight()-200, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		// second pass
		// glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		// glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
		// glClear(GL_COLOR_BUFFER_BIT);

		glDepthFunc(GL_ALWAYS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// bind opaque framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);

		// use composite shader
		compositeShader.bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, accumTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, revealTexture);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);




		
		// draw to backbuffer (final pass)
		// -----

		// set render states
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE); // enable depth writes so glClear won't ignore clearing the depth buffer
		glDisable(GL_BLEND);

		// bind backbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.6, 0.7, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// use screen shader
		screenShader.bind();

		// draw final screen quad
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, opaqueTexture);
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		
		
/* 		//screenShader.bind();
		glDepthFunc(GL_ALWAYS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderer.render_to_frame_buffer();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, accumTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, revealTexture);
 */
		//renderer.render_to_screen();
		//frame_buffer->unbind();
/* 		glBindVertexArray(quadVAO);
		//quad_vertex_buffer.bind();
		GLCALL(glDisable(GL_DEPTH_TEST));
		GLCALL(glBindTexture(GL_TEXTURE_2D, frame_buffer->get_texture_id()));
		glDrawArrays(GL_TRIANGLES, 0, 6);  */
		
		//quad_vertex_buffer.unbind();
		//screenShader.unbind();  
		
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