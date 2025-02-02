#pragma once
#include <vector>

#include "definitions.hpp"
#include "glm/glm.hpp"

#include <iostream>
#include "printFunktions.hpp"
#include "util_funcs.hpp"
#include "shader.hpp"
#include "textures.hpp" 
#include "SDL_handler.hpp"


#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

// forward declerations

struct ChunkVertexBuffer;
struct ColorVertexBuffer;
struct IndexBuffer;



class Mesh{
protected:
	std::vector<Vertex> vertices;
	uint32 usedVertices = 0;
	std::vector<uint32> indices;
	uint32 usedIndices = 0;
public:
	// change to Pointer
	ChunkVertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	Mesh();
	~Mesh();
	uint32 getNumIndices();
	uint32 getNumVertices();
	Vertex* getVertices();
	uint32* getIndices();

	void reserveVertices(int numVertices);
	void reserveIndices(int numIndices);

	virtual void initialize();

	virtual void render(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId);

	virtual void update();
	virtual void clearMesh();

};

class ChunkMesh: public Mesh {
private:
	int textureAtlasSize = 16; //TODO: Use a Global Var / get()
	float tex_factor = 1.0f / textureAtlasSize;

public:
	ChunkMesh();
	~ChunkMesh();
	void reserveFaces(int numFaces);
	void addPlane(glm::vec3 position, int rotation = 0, int tex_x = 0, int tex_y = 1, int size = 1);
	void addPlane_basic_lighting(glm::vec3 position, BlockDirection rotation = Z_positive, int tex_x = 0, int tex_y = 0,
                                 int size = 1, int face_rot = 0);
	void add_Block(glm::vec3 position, int tex_x = 0, int tex_y = 1);

    void addPlane_basic_lighting_with_offset(glm::vec3 position, int rotation, int tex_x, int tex_y, float size);

    void add_Block_with_offset(glm::vec3 position, int tex_x, int tex_y, float size);

    void add_cross(glm::vec3 position, int tex_x, int tex_y, float size);

    void diagonal_Plane(glm::vec3 position, int tex_x, int tex_y, float size, int c);
};


class Box {
public:
	Box(const char* VertexShaderFilename, const char* fragmentShaderFilename,  const char* TextureFilename);
	int getNumIndices();
	int getNumVertices();
	ColorVertex* getVertices();
	uint32* getIndices();
	void render(int x, int y, int z, const GLfloat* modelViewProj);

protected:
	uint32 numVertices = 24;
	ColorVertex vertices[24] = {
		ColorVertex{0.0f, -0.001f, 1.0f, // front
		0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, -0.001f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{-0.001f, 1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f},

		
		ColorVertex{1.0f, -0.001f, 1.0f, // right
		0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, -0.001f, -0.001f,
		1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, -0.001,
		1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 1.0,
		0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		
		
		ColorVertex{1.0f, -0.001f, -0.001f, // back
		0.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		ColorVertex{-0.001f, -0.001f, -0.001,
		1.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		ColorVertex{-0.001f, 1.0f, -0.001,
		1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, -0.001,
		0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		
		ColorVertex{-0.001f, -0.001f, -0.001f, // left
		0.0f, 0.0f,
		0.0, 1.0f, 0.0f, 1.0f},
		ColorVertex{-0.001f, -0.001f, 1.0f,
		1.0f, 0.0f,
		0.0, 1.0f, 0.0f, 1.0f},
		ColorVertex{-0.001f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0, 1.0f, 0.0f, 1.0f},
		ColorVertex{-0.001f, 1.0f, -0.001,
		0.0f, 1.0f,
		0.0, 1.0f, 0.0f, 1.0f},
		
		ColorVertex{-0.001f, -0.001f, -0.001f, // bottom
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, -0.001f, -0.001f,
		1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, -0.001f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{-0.001f, -0.001f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f},
		
		ColorVertex{-0.001f, 1.0f, 1.0f, // top
		0.0f, 0.0f,
		1.0, 1.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		1.0, 1.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, -0.001,
		1.0f, 1.0f,
		1.0, 1.0f, 1.0f, 1.0f},
		ColorVertex{-0.001f, 1.0f, -0.001,
		0.0f, 1.0f,
		1.0, 1.0f, 1.0f, 1.0f},	
		
	};
	
	uint32 numIndices = 36;
	uint32 indices[36] = {
		0, 1, 2,//front
		0, 2, 3,

		
		4, 5, 6,//right
		4, 6, 7,
		
		8, 9, 10,//back
		8, 10, 11,
		
		12, 13, 14,//left
		12, 14, 15,
		
		16, 17, 18,//bottom
		16, 18, 19,
		
		20, 21, 22,//top
		20, 22, 23
		

	};
	
	ColorVertexBuffer vertex_buffer = ColorVertexBuffer(vertices, numVertices);
	IndexBuffer index_buffer = IndexBuffer(indices, numIndices, sizeof(indices[0]));
	int PositionUniformLocation;
	int modelViewProjMatrixLocation;
	Shader shader = Shader();
	Texture texture;	
};

class Skybox:Box{
protected:
	ColorVertex vertices[24] = {
		ColorVertex{-0.1f, 0.0f, 1.0f, // front
		0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, 0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{-0.1f, 1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f},

		
		ColorVertex{1.0f, 0.0f, 1.0f, // right
		0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 0.0,
		1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 1.0,
		0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		
		
		ColorVertex{1.0f, 0.0f, 0.0f, // back
		0.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		ColorVertex{0.0f, 0.0f, 0.0,
		1.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		ColorVertex{0.0f, 1.0f, 0.0,
		1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 0.0,
		0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		
		ColorVertex{0.0f, 0.0f, 0.0f, // left
		0.0f, 0.0f,
		0.0, 1.0f, 0.0f, 1.0f},
		ColorVertex{0.0f, 0.0f, 1.0f,
		1.0f, 0.0f,
		0.0, 1.0f, 0.0f, 1.0f},
		ColorVertex{0.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0, 1.0f, 0.0f, 1.0f},
		ColorVertex{0.0f, 1.0f, 0.0,
		0.0f, 1.0f,
		0.0, 1.0f, 0.0f, 1.0f},
		
		ColorVertex{0.0f, 0.0f, 0.0f, // bottom
		0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, 0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f},
		
		ColorVertex{0.0f, 1.0f, 1.0f, // top
		0.0f, 0.0f,
		1.0, 1.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		1.0, 1.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 0.0,
		1.0f, 1.0f,
		1.0, 1.0f, 1.0f, 1.0f},
		ColorVertex{0.0f, 1.0f, 0.0,
		0.0f, 1.0f,
		1.0, 1.0f, 1.0f, 1.0f},	
		
	};
public:

	Skybox(const char* VertexShaderFilename, const char* fragmentShaderFilename,  
		   const char* TextureFilename);
	void render(float x, float y, float z, const GLfloat* modelViewProj);
};




struct Quad{
	 float vertices[30] = {
		// positions		// uv
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,

		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};
    unsigned int quadVAO, quadVBO;
	Quad();
	virtual void draw();
};



struct SkyQuad:Quad{
	uint32 lookAt_uniformLocation;
	uint32 InvProjection_uniformLocation;
    uint32 InvView_uniformLocation;
	Shader shader = Shader();
	//uint32 sunPosition_uniformLocation;
	//uint32 color_uniformLocation;
	SkyQuad(const char* VertexShaderFilename, const char* fragmentShaderFilename);
	void render(glm::vec3 lookAt, const GLfloat* InvProjection, const GLfloat* InvView);
};








class UImesh{
public:
    explicit UImesh(int texture_size=16);
    ~UImesh();
    void reserveVertices(int numVertices);
    void reserveIndices(int numIndices);
    uint32 getNumIndices();
    uint32 getNumVertices();
    Vertex* getVertices();
    uint32* getIndices();

    void update();
    void addQuad(glm::vec3 position, int rotation = 0, int tex_x = 0, int tex_y = 0, float scale = 1);

    void clearMesh();

    void render(GLuint textureId);
protected:
	std::vector<Vertex> vertices;
	uint32 usedVertices = 0;

	std::vector<uint32> indices;
	uint32 usedIndices = 0;

	int textureAtlasSize; //TODO: Use a Global Var / get()
	float tex_factor;
	ChunkVertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	float aspect_ratio;
/////////////////////////////////////////////////////

};


glm::vec3 calculate_slot_position(int slot_number);

class ItemBarMesh: public UImesh{
public:
	explicit ItemBarMesh(int texture_size=4);
    ~ItemBarMesh() = default;
};

class InventoryMesh{
public:
    InventoryMesh();

    void update();

    void setSlot(int slot_number);

    void render(GLuint UItextureId, GLuint BlocktextureId);
	ItemBarMesh itemBar = ItemBarMesh(4);
	UImesh slotSelector = UImesh(4);
	UImesh items;
	int last_selected_slot=5;
};



class TextMesh: public Mesh {
private:
	float sizeX;
	float sizeY;


public:
	TextMesh(float sx, float sy);
	~TextMesh();
	void reserveCharacters(int numChars);
	void addCharacter(glm::vec2 position, int rotation = 0, float x = 0, float y = 0, float scale_x = 0.1, float scale_y = 0.1);
	void render(GLuint texture_id);
};
