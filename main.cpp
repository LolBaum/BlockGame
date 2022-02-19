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



#include "definitions.h"
#include "Vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"
#include "object_models.h"
#include "floating_camera.h"
#include "printFunktions.h"
#include "util_funcs.h"

#include "local_player_data.h"

#include "Block.h"
#include "Chunk.h"
#include "SuperChunk.h"

#include "SDL_handler.h"
//#include <filesystem>
#include <sstream>
//namespace fs = std::filesystem; // Needs C++ 17 or above 


// Work in Progress Section
//#include <SDL_image.h> 
void draw_text(const char* msg, int x, int y, int r, int g, int b, int size){
	SDL_Surface* surf;
	SDL_Texture* tex;
	//TTF_Font
}

// freetype
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;



void RenderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color, int VAO, int VBO)
{
    // activate corresponding render state	
    //s.Use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	s.bind();
    //glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(s.getShaderId(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	s.unbind();
	glDisable(GL_BLEND);
}


// String splitting from Stackoverflow https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

void render_multiline_text(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color, int VAO, int VBO, float f_size, float line_offset=5){
	std::string delimiter = "\n";
	std::string line;
	size_t last = 0; 
	size_t next = 0; 
	int i = 0;
	while ((next = text.find(delimiter, last)) != std::string::npos){
		line = text.substr(last, next-last);
		RenderText(s, line, x, y-(line_offset+f_size*scale)*i, scale, color, VAO, VBO);
		last = next + 1;
		i++;
	}
}
// -------------------------------------------------------



// Main Program

int main_function() {
	//btm.AddBlockType(BlockType(0, 3, 1));
	//BlockType b = *btm.GetBlockType(0);

	//b.printInfo();

	

	/*
	std::cout << "Starting Program" << std::endl;
	SDL_Window* window;
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetSwapInterval(1); // Vsync 

#ifdef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	uint32 flags = SDL_WINDOW_OPENGL;

	window = SDL_CreateWindow("C++ OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, flags);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	*/
	
	
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




	////Plane obj = Plane();
	//Cube_old obj = Cube_old();

	// based on the turorial from learnopengl.com
	// https://learnopengl.com/In-Practice/Text-Rendering

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "fonts/TheJewishBitmap.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
		return -1;
	}

	float font_size = 40;

	FT_Set_Pixel_Sizes(face, 0, font_size);
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;  
		return -1;
	}




	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture, 
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	

	glm::mat4 font_projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader font_shader("shaders/font.vs", "shaders/font.fs");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(sdl_handler.getWidth()), 0.0f, static_cast<float>(sdl_handler.getHeight()));
    font_shader.bind();
    glUniformMatrix4fv(glGetUniformLocation(font_shader.getShaderId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));



	// int colorUniformLocation = glGetUniformLocation(font_shader.getShaderId(), "u_color");
	// if (colorUniformLocation != -1) {
	// 	GLCALL(glUniform4f(colorUniformLocation, 1.0f, 0.0f, 1.0f, 1.0f));
	// }
	// int TextureUniformLocation = glGetUniformLocation(font_shader.getShaderId(), "u_texture");
	// if (TextureUniformLocation != -1) {
	// 	GLCALL(glUniform1i(TextureUniformLocation, 0));
	// }
	// int modelViewProjMatrixLocation = GLCALL(glGetUniformLocation(font_shader.getShaderId(), "u_modelViewProj"));
	

	/*
	obj.addPlane(glm::vec3(0, 0, 0));
	obj.addPlane(glm::vec3(0, 0, 0), 1);
	obj.addPlane(glm::vec3(0, 0, 0), 2);
	obj.addPlane(glm::vec3(0, 0, 0), 3);
	obj.addPlane(glm::vec3(0, 0, 0), 4);
	obj.addPlane(glm::vec3(0, 0, 0), 5);
	obj.addPlane(glm::vec3(0, 2.f, -1));
	*/

	//obj.add_Block(glm::vec3(1, 1, 0));

	//std::cout << "size of ColorVertex: " << sizeof(ColorVertex) << std::endl;
	//std::cout << "size" << sizeof(ColorVertex) * 16 * 16 * 16 * 24 << std::endl;
	//int c = 0;
	//for (int i = 0; i < 10; i++) {
	//	for (int j = 0; j <= i; j++) {
	//		for (int k = 0; k <= j; k++) {
	//			obj.add_grass_Block(glm::vec3(j,k, -i));
	//			c++;
	//		}

	//	}

	//}
	//for (int x = 0; x < 60; x++) {
	//	for (int z = 0; z < 60; z++) {
	//		float a = (glm::perlin(glm::vec2((x) / 16.0, (z) / 16.0))+1)/2;
	//		float b = (glm::perlin(glm::vec2((x) / 32.0, (z) / 32.0)) + 1) /2*128;
	//		int h = a * b;
	//		std::cout << "h: " << h << endl;
	//		//std::cout << "b: " << b << endl;
	//	}

	//}

	



	
	int s = 4;



		//for (int i = 0; i < s; i++) {
		//	for (int j = 0; j < s; j++) {
		//		chunkManager.loadChunk(i * 16, 0, j * 16);
		//		//chunkManager.unloadChunk(i * 16, 0, j * 16);
		//	}

		//}



	LocalPlayer player = LocalPlayer();

	//using Stacks for vertical World Inforamtion e.g. height...

	//player.update();
	//chunkManager.loadChunks(player.getLocalChunkIds());
	//glm::vec3 p = player.getPosition();
	//chunkManager.getChunk(player.getCurrentChunkPos());


	//for (int i = 0; i < 4000; i++) {
	//	Chunk* chunk = new Chunk();
	//	chunk->dummyWorldGen(5);
	//	chunk->updateMesh();
	//	chunk->~Chunk();
	//}
	//
	//std::cout << std::endl << std::endl << "Chunk: " << chunkManager.getChunk(glm::vec3(0,0,16))->getPos().x << std::endl;

	//std::vector<Chunk*> chunks = chunkManager.getChunks();


	//std::vector<Chunk*> chunks;


	//Chunk chunk = Chunk();
	////chunk.setBlock(0, 0, 0, 1);
	//chunk.dummyWorldGen(1);
	//chunk.updateMesh();
	//

	//Chunk chunk2 = Chunk(glm::vec3(0, 16, 0));
	//chunk2.dummyWorldGen(2);
	//chunk2.updateMesh();

	//Chunk chunk3 = Chunk(glm::vec3(16, 16, 0));
	//chunk3.dummyWorldGen(3);
	//chunk3.updateMesh();

	//Chunk chunk4 = Chunk(glm::vec3(16,0,0));
	//chunk4.dummyWorldGen(4);
	//chunk4.updateMesh();

	//chunks.push_back(&chunk);
	//chunks.push_back(&chunk2);
	//chunks.push_back(&chunk3);
	//chunks.push_back(&chunk4);


	//ChunkMesh obj = *chunk.getMesh();
	////std::cout << "size of mesh: " << obj.getVertices().size() << std::endl;

	//uint32 numVertices = obj.getNumVertices();
	//Vertex* vertices = obj.getVertices();


	//uint32 numIndices = obj.getNumIndices();
	//uint32* indices = obj.getIndices();

	//uint32 numVertices = 4;
	//Vertex* vertices = obj.dummy_get();


	//uint32 numIndices = 6;
	//uint32 indices[6] = {
	//0, 1, 2,//front
	//0, 2, 3};

	


	////IndexBuffer indexBuffer(indices, numIndices, sizeof(indices[0]));
	//IndexBuffer indexBuffer = IndexBuffer();
	//indexBuffer.update(indices, numIndices, sizeof(indices[0]));

	///*ChunkVertexBuffer vertexBuffer(&vertices [0], numVertices);*/
	//ChunkVertexBuffer vertexBuffer = *new ChunkVertexBuffer();
	////vertexBuffer.update(&vertices[0], numVertices);
	////vertexBuffer.unbind();

	////chunk.setIndexBuffer(indexBuffer);
	////chunk.setVertexBuffer(vertexBuffer);
	//
	//IndexBuffer indexBuffer1 = *chunk.getIndexBuffer();
	//indexBuffer1.update(indices, numIndices, sizeof(indices[0]));

	//ChunkVertexBuffer vertexBuffer1 = *chunk.getVertexBuffer();
	//vertexBuffer1.update(&vertices[0], numVertices);
	//vertexBuffer1.unbind();
	//float32 pos_x = 8.0;
	//float32 pos_y = 28.0;
	//float32 pos_z = 100.0;
	//std::vector<Vertex> obj;
	//obj.push_back(Vertex{ -10.0f + pos_x, -10.0f + pos_y, pos_z,
	//	0.0f, 0.0f,
	//	1.0f });
	//obj.push_back(Vertex{ 10.0f + pos_x, -10.0f + pos_y,pos_z,
	//	1.0f, 0.0f,
	//	1.0f });
	//obj.push_back(Vertex{ 10.0f + pos_x, 10.0f + pos_y, pos_z,
	//	1.0f, 1.0f,
	//	1.0f });
	//obj.push_back(Vertex{ -10.0f + pos_x, 10.0f + pos_y,  pos_z,
	//	0.0f, 1.0f,
	//	1.0 });

	//obj.push_back(Vertex{ -10.0f + pos_x, -10.0f + pos_y, -10.0f + pos_z,
	//0.0f, 0.0f,
	//1.0f });
	//obj.push_back(Vertex{ 10.0f + pos_x, -10.0f + pos_y, -10.0f + pos_z,
	//	1.0f, 0.0f,
	//	1.0f });
	//obj.push_back(Vertex{ 10.0f + pos_x, -10.0f + pos_y,  10.0f + pos_z,
	//	1.0f, 1.0f,
	//	1.0f });
	//obj.push_back(Vertex{ -10.0f + pos_x, -10.0f + pos_y, 10.0f + pos_z,
	//	0.0f, 1.0f,
	//	1.0 });
	//uint32 numVertices = obj.size();
	//Vertex* vertices = obj.data();

	//uint32 numIndices = 12;
	//uint32 indices[12] = {
	//	0, 1, 2,//front
	//	0, 2, 3,
	//	4, 5, 6,
	//	4, 6, 7};

	//IndexBuffer indexBuffer(indices, numIndices, sizeof(indices[0]));

	//ChunkVertexBuffer vertexBuffer(vertices, numVertices);
	//vertexBuffer.unbind();

	
	
	/*
	uint32 numVertices = obj.getNumVertices();
	ColorVertex *vertices = obj.getVertices();

	uint32 numIndices = obj.getNumIndices();
	uint32 *indices = obj.getIndices();


	IndexBuffer indexBuffer(indices, numIndices, sizeof(indices[0]));

	ColorVertexBuffer vertexBuffer(vertices, numVertices);
	vertexBuffer.unbind();
	*/

	int32 textureWidth = 0;
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
	}


	Shader shader("shaders/basic.vs", "shaders/basic.fs");
	shader.bind();

	uint64 perfCounterFrequency = SDL_GetPerformanceFrequency();
	uint64 lastCounter = SDL_GetPerformanceCounter();
	float32 delta = 0.0f;

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);

	//glEnable(GL_NORMALIZE);

	int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
	if (colorUniformLocation != -1) {
		GLCALL(glUniform4f(colorUniformLocation, 1.0f, 0.0f, 1.0f, 1.0f));
	}

	int TextureUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_texture");
	if (TextureUniformLocation != -1) {
		GLCALL(glUniform1i(TextureUniformLocation, 0));
	}

	//glm::mat4 model = player.model;


	//FloatingCamera camera = player.camera;


	//glm::mat4 modelViewProj = player.modelViewProj;

	int modelViewProjMatrixLocation = GLCALL(glGetUniformLocation(shader.getShaderId(), "u_modelViewProj"));






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
	bool buttonShift = false;
	bool buttonZ = false;
	bool mouseButtonL = false;
	bool mouseButtonR = false;
	bool mouseButtonClickL = false;
	bool mouseButtonClickR = false;

	bool close = false;
	sdl_handler.SetRelativeMouseMode(true); // in Init ?
	float cameraSpeed = 6.0f;
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
					break;
				case SDLK_LSHIFT:
					buttonShift = true;
					break;
				case SDLK_z:
					buttonZ = true;
					GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
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

		if (buttonW) {
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
		}
		
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
		
		//model = glm::rotate(model, 1.0f * delta, glm::vec3(0, 1, 0));
		//modelViewProj = player.modelViewProj;


		//const GLfloat* dummyModelViewProj = (const GLfloat*) &modelViewProj[0][0];
		
		//vertexBuffer.bind();
		//indexBuffer.bind();
		//GLCALL(glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, player.getModelViewProj_GL()));
		//GLCALL(glActiveTexture(GL_TEXTURE0));
		//GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
		//GLCALL(glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0));
		//indexBuffer.unbind();
		//vertexBuffer.unbind();

		
		//renderChunk(modelViewProjMatrixLocation, dummyModelViewProj, textureId, numIndices, &vertexBuffer, &indexBuffer);
		//renderChunk(modelViewProjMatrixLocation, dummyModelViewProj, textureId, numIndices, &vertexBuffer1, &indexBuffer1);

		//renderChunk2(modelViewProjMatrixLocation, dummyModelViewProj, textureId, &chunk);

		//renderChunk(modelViewProjMatrixLocation, dummyModelViewProj, textureId, numIndices, chunk.getVertexBuffer(), chunk.getIndexBuffer());

		//chunk.render(modelViewProjMatrixLocation, (const GLfloat*) &modelViewProj[0][0], textureId); // wrong Pointers?
		//chunk2.render(modelViewProjMatrixLocation, (const GLfloat*)&modelViewProj[0][0], textureId);
		//chunk3.render(modelViewProjMatrixLocation, (const GLfloat*)&modelViewProj[0][0], textureId);
		//chunk4.render(modelViewProjMatrixLocation, (const GLfloat*)&modelViewProj[0][0], textureId);

		//for (int i = 0; i < chunks.size(); i++) {
		//	chunks.at(i)->render(modelViewProjMatrixLocation, (const GLfloat*)&modelViewProj[0][0], textureId);
		//}
		shader.bind();
		chunkManager.render(modelViewProjMatrixLocation, player.getModelViewProj_GL(), textureId);
		shader.unbind();
		//for (int i = 0; i < chunks.size(); i++) {
		//	chunks.at(i)->render(modelViewProjMatrixLocation, (const GLfloat*)&modelViewProj[0][0], textureId);
		//	//std::cout << "renderd Chunk NR " << i << " at " << chunks.at(i)->getPos().x << ", " << chunks.at(i)->getPos().y << ", " << chunks.at(i)->getPos().z << ", " << std::endl;
		//}


		// render fonts
		
		render_multiline_text(font_shader, text, 25, sdl_handler.getHeight()-50, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), VAO, VBO, font_size);


		sdl_handler.SDL_SwapWindow();
		//SDL_GL_SwapWindow(window); 
		//SDL_handler::SDL_SwapWindow();


		// include in SDL_handler
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
			ss << "Number of Chunks: "<< chunkManager.getNumChunks() << "  " << chunkManager.getNumFilledChunks() << std::endl;
			ss << vec3_toString(player.getPosition(), "pos: ") << std::endl;
			text = ss.str();
			time_since_slow_tick = 0;
		}
		//std::cout << FPS << std::endl;
		lastCounter = endCounter;

		mouseButtonClickL = false;
		mouseButtonClickR = false;
	}

	GLCALL(glDeleteTextures(1, &textureId));

	return 0;
}

int main(int argc, char** argv) {
	std::cout << "Starting Program" << std::endl;
	int r;
	r = main_function();

	std::cout << "Program exits" << std::endl;
	return r;
}