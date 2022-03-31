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
//#include "Block.hpp"



class ChunkMesh {
private:
	uint32 numMaxVertices = 16 * 16 * 16 * 24;
	std::vector<Vertex> vertices;
	uint32 usedVertices = 0;

	uint32 numMaxIndices = 16 * 16 * 16 * 36;
	std::vector<uint32> indices;
	uint32 usedIndices = 0;

	int textureAtlasSize = 4;
	float tex_factor = 1.0f / textureAtlasSize;

public:
	std::vector<Vertex> obj;
	ChunkVertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	ChunkMesh() {
		
		vertexBuffer = *new ChunkVertexBuffer();
		indexBuffer = *new IndexBuffer();
		
	}
	~ChunkMesh() {
		std::vector<Vertex>().swap(vertices);
		std::vector<uint32>().swap(indices);
		vertexBuffer.~ChunkVertexBuffer();
		indexBuffer.~IndexBuffer();
	}

	uint32 getNumIndices() {return usedIndices;}
	uint32 getNumVertices() {return usedVertices;}
	Vertex* getVertices() {return vertices.data();}
	uint32* getIndices() {return indices.data();}
	
	void reserveVertices(int numVertices) {
		vertices.reserve(numVertices);
	}
	void reserveIndices(int numIndices) {
		indices.reserve(numIndices);
	}
	void reserveFaces(int numFaces) {
		reserveVertices(numFaces*4);
		reserveIndices(numFaces * 6);
		// could return Int as Warning
	}

	void render(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId) {
		//makes fully transparent images work
		/* glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc (GL_GREATER, 0.1);
		glEnable(GL_ALPHA_TEST); */
		
		vertexBuffer.bind();
		indexBuffer.bind();

		GLCALL(glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, modelViewProj));
		GLCALL(glActiveTexture(GL_TEXTURE0));
		GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
		GLCALL(glDrawElements(GL_TRIANGLES, usedIndices, GL_UNSIGNED_INT, 0));

		indexBuffer.unbind();
		vertexBuffer.unbind();
	}

	void update(){
		//Vertex* vertices = getVertices();
		//uint32* indices = getIndices();
		indexBuffer.update(getIndices(), usedIndices, sizeof(getIndices()[0])); // fix sizeof
		vertexBuffer.update(&getVertices()[0], usedVertices);

	}

	
	// rotation: front-0, right-1, back-2, left-3, buttom-4, top-5,
	void addPlane(glm::vec3 position, int rotation = 0, int tex_x = 0, int tex_y = 1, int size = 1) {

		float uv_x_1 = tex_x * tex_factor;
		float uv_x_2 = tex_x * tex_factor + tex_factor;
		float uv_y_1 = 1.0f - tex_y * tex_factor - tex_factor;
		float uv_y_2 = 1.0f - tex_y * tex_factor;

		indices.push_back(usedVertices + 0);
		indices.push_back(usedVertices + 1);
		indices.push_back(usedVertices + 2);
		indices.push_back(usedVertices + 0);
		indices.push_back(usedVertices + 2);
		indices.push_back(usedVertices + 3);
		usedIndices += 6;

		switch (rotation)
		{
		case 0: {

			vertices.push_back(Vertex{ position.x + -0.5f, position.y + -0.5f, position.z + 0.5f,
						uv_x_1, uv_y_1,
						1.0});
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + -0.5f, position.z + 0.5f,
							uv_x_2, uv_y_1,
							1.0 });
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + 0.5f, position.z + 0.5f,
							uv_x_2, uv_y_2,
							1.0 });
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + 0.5f, position.z + 0.5f,
							uv_x_1, uv_y_2,
							1.0 });
			break;
		}
		case 1: {
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + -0.5f, position.z + 0.5f,
						uv_x_1, uv_y_1,
						1.0 });
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + -0.5f, position.z + -0.5f,
							uv_x_2, uv_y_1,
							1.0 });
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_2, uv_y_2,
							1.0 });
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + 0.5f, position.z + 0.5f,
							uv_x_1, uv_y_2,
							1.0 });
			break;
		}
		case 2: {
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + -0.5f, position.z + -0.5f,
						uv_x_1, uv_y_1,
						1.0 });
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + -0.5f, position.z + -0.5f,
							uv_x_2, uv_y_1,
							1.0 });
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_2, uv_y_2,
							1.0 });
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_1, uv_y_2,
							1.0 });
			break;
		}
		case 3: {
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + -0.5f, position.z + -0.5f,
						uv_x_1, uv_y_1,
						1.0 });
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + -0.5f, position.z + 0.5f,
							uv_x_2, uv_y_1,
							1.0 });
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + 0.5f, position.z + 0.5f,
							uv_x_2, uv_y_2,
							1.0 });
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_1, uv_y_2,
							1.0 });
			break;
		}
		case 4: {
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + -0.5f, position.z + -0.5f,
						uv_x_1, uv_y_1,
						1.0 });
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + -0.5f, position.z + -0.5f,
							uv_x_2, uv_y_1,
							1.0 });
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + -0.5f, position.z + 0.5f,
							uv_x_2, uv_y_2,
							1.0 });
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + -0.5f, position.z + 0.5f,
							uv_x_1, uv_y_2,
							1.0 });
			break;
		}
		case 5: {
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + 0.5f, position.z + 0.5f,
						uv_x_1, uv_y_1,
						1.0 });
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + 0.5f, position.z + 0.5f,
							uv_x_2, uv_y_1,
							1.0 });
			vertices.push_back(Vertex{ position.x + 0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_2, uv_y_2,
							1.0 });
			vertices.push_back(Vertex{ position.x + -0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_1, uv_y_2,
							1.0 });
			break;
		}

		default:
			break;
		}
		usedVertices += 4;
	}

	void addPlane_basic_lighting(glm::vec3 position, int rotation = 0, int tex_x = 0, int tex_y = 0, int size = 1) {

		float uv_x_1 = tex_x * tex_factor;
		float uv_x_2 = tex_x * tex_factor + tex_factor;
		float uv_y_1 = 1.0f - tex_y * tex_factor - tex_factor;
		float uv_y_2 = 1.0f - tex_y * tex_factor;

		indices.push_back(usedVertices + 0);
		indices.push_back(usedVertices + 1);
		indices.push_back(usedVertices + 2);
		indices.push_back(usedVertices + 0);
		indices.push_back(usedVertices + 2);
		indices.push_back(usedVertices + 3);
		usedIndices += 6;

		switch (rotation)
		{
		case 0: {

			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 1.0f,
						uv_x_1, uv_y_1,
						0.7 });
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 1.0f,
							uv_x_2, uv_y_1,
							0.7 });
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 1.0f,
							uv_x_2, uv_y_2,
							0.7 });
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 1.0f,
							uv_x_1, uv_y_2,
							0.7 });
			break;
		}
		case 1: {
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 1.0f,
						uv_x_1, uv_y_1,
						0.8 });
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 0.0f,
							uv_x_2, uv_y_1,
							0.8 });
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 0.0f,
							uv_x_2, uv_y_2,
							0.8 });
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 1.0f,
							uv_x_1, uv_y_2,
							0.8 });
			break;
		}
		case 2: {
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 0.0f,
						uv_x_1, uv_y_1,
						0.7 });
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 0.0f,
							uv_x_2, uv_y_1,
							0.7 });
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 0.0f,
							uv_x_2, uv_y_2,
							0.7 });
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 0.0f,
							uv_x_1, uv_y_2,
							0.7 });
			break;
		}
		case 3: {
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 0.0f,
						uv_x_1, uv_y_1,
						0.8 });
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 1.0f,
							uv_x_2, uv_y_1,
							0.8 });
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 1.0f,
							uv_x_2, uv_y_2,
							0.8 });
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 0.0f,
							uv_x_1, uv_y_2,
							0.8 });
			break;
		}
		case 4: {
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 0.0f,
						uv_x_1, uv_y_1,
						0.6 });
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 0.0f,
							uv_x_2, uv_y_1,
							0.6 });
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 1.0f,
							uv_x_2, uv_y_2,
							0.6 });
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 1.0f,
							uv_x_1, uv_y_2,
							0.6 });
			break;
		}
		case 5: {
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 1.0f,
						uv_x_1, uv_y_1,
						1.0 });
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 1.0f,
							uv_x_2, uv_y_1,
							1.0 });
			vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 0.0f,
							uv_x_2, uv_y_2,
							1.0 });
			vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 0.0f,
							uv_x_1, uv_y_2,
							1.0 });
			break;
		}

		default:
			break;
		}
		usedVertices += 4;
	}

	void add_Block(glm::vec3 position) {
		addPlane(glm::vec3(0, 0, 0) + position);
		addPlane(glm::vec3(0, 0, 0) + position, 1);
		addPlane(glm::vec3(0, 0, 0) + position, 2);
		addPlane(glm::vec3(0, 0, 0) + position, 3);
		addPlane(glm::vec3(0, 0, 0) + position, 4);
		addPlane(glm::vec3(0, 0, 0) + position, 5);
	}
	void add_grass_Block(glm::vec3 position) {
		addPlane(glm::vec3(0, 0, 0) + position, 0, 2, 0);
		addPlane(glm::vec3(0, 0, 0) + position, 1, 2, 0);
		addPlane(glm::vec3(0, 0, 0) + position, 2, 2, 0);
		addPlane(glm::vec3(0, 0, 0) + position, 3, 2, 0);
		addPlane(glm::vec3(0, 0, 0) + position, 4, 1, 0);
		addPlane(glm::vec3(0, 0, 0) + position, 5, 3, 0);
	}

	void clearMesh() {
		vertices.clear();
		indices.clear();
		usedIndices = 0;
		usedVertices = 0;
	}
};


class SimpleChunkMesh {
public:
	SimpleChunkMesh() {

	}
	uint32 getNumIndices() {
		return usedIndices;
	}
	uint32 getNumVertices() {
		return usedVertices;
	}
	ColorVertex* getVertices() {
		return vertices;
	}
	uint32* getIndices() {
		return indices;
	}

	void addPlane(glm::vec3 position, int rotation = 0, int tex_x = 0, int tex_y=1, int size = 1) {
		float uv_x_1 = tex_x * tex_factor;
		float uv_x_2 = tex_x * tex_factor + tex_factor;
		float uv_y_1 = 1.0f - tex_y * tex_factor - tex_factor;
		float uv_y_2 = 1.0f - tex_y * tex_factor;

		indices[usedIndices + 0] = usedVertices + 0;
		indices[usedIndices + 1] = usedVertices + 1;
		indices[usedIndices + 2] = usedVertices + 2;
		indices[usedIndices + 3] = usedVertices + 0;
		indices[usedIndices + 4] = usedVertices + 2;
		indices[usedIndices + 5] = usedVertices + 3;
		usedIndices += 6;

		switch (rotation)
		{
		case 0:{

			vertices[usedVertices] = ColorVertex{ position.x + -0.5f, position.y + -0.5f, position.z + 0.5f,
						uv_x_1, uv_y_1,
						0.5f, 0.0f, 0.0f, 0.5f };
			vertices[usedVertices + 1] = ColorVertex{ position.x + 0.5f, position.y + -0.5f, position.z + 0.5f,
							uv_x_2, uv_y_1,
							0.0f, 0.0f, 0.5f, 0.5f };
			vertices[usedVertices + 2] = ColorVertex{ position.x + 0.5f, position.y + 0.5f, position.z + 0.5f,
							uv_x_2, uv_y_2,
							0.0f, 0.5f, 0.5f, 0.5f };
			vertices[usedVertices + 3] = ColorVertex{ position.x + -0.5f, position.y + 0.5f, position.z + 0.5f,
							uv_x_1, uv_y_2,
							0.0, 0.5f, 0.0f, 0.5f };
			break;
		}
		case 1: {
			vertices[usedVertices] = ColorVertex{ position.x + 0.5f, position.y + -0.5f, position.z + 0.5f,
						uv_x_1, uv_y_1,
						0.5f, 0.0f, 0.0f, 0.5f };
			vertices[usedVertices + 1] = ColorVertex{ position.x + 0.5f, position.y + -0.5f, position.z + -0.5f,
							uv_x_2, uv_y_1,
							0.0f, 0.0f, 0.5f, 0.5f };
			vertices[usedVertices + 2] = ColorVertex{ position.x + 0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_2, uv_y_2,
							0.0f, 0.5f, 0.5f, 0.5f };
			vertices[usedVertices + 3] = ColorVertex{ position.x + 0.5f, position.y + 0.5f, position.z + 0.5f,
							uv_x_1, uv_y_2,
							0.0, 0.5f, 0.0f, 0.5f };
			break;
		}
		case 2: {
			vertices[usedVertices] = ColorVertex{ position.x + 0.5f, position.y + -0.5f, position.z + -0.5f,
						uv_x_1, uv_y_1,
						0.5f, 0.0f, 0.0f, 0.5f };
			vertices[usedVertices + 1] = ColorVertex{ position.x + -0.5f, position.y + -0.5f, position.z + -0.5f,
							uv_x_2, uv_y_1,
							0.0f, 0.0f, 0.5f, 0.5f };
			vertices[usedVertices + 2] = ColorVertex{ position.x + -0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_2, uv_y_2,
							0.0f, 0.5f, 0.5f, 0.5f };
			vertices[usedVertices + 3] = ColorVertex{ position.x + 0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_1, uv_y_2,
							0.0, 0.5f, 0.0f, 0.5f };
			break;
		}
		case 3: {
			vertices[usedVertices] = ColorVertex{ position.x + -0.5f, position.y + -0.5f, position.z + -0.5f,
						uv_x_1, uv_y_1,
						0.5f, 0.0f, 0.0f, 0.5f };
			vertices[usedVertices + 1] = ColorVertex{ position.x + -0.5f, position.y + -0.5f, position.z + 0.5f,
							uv_x_2, uv_y_1,
							0.0f, 0.0f, 0.5f, 0.5f };
			vertices[usedVertices + 2] = ColorVertex{ position.x + -0.5f, position.y + 0.5f, position.z + 0.5f,
							uv_x_2, uv_y_2,
							0.0f, 0.5f, 0.5f, 0.5f };
			vertices[usedVertices + 3] = ColorVertex{ position.x + -0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_1, uv_y_2,
							0.0, 0.5f, 0.0f, 0.5f };
			break;
		}
		case 4: {
			vertices[usedVertices] = ColorVertex{ position.x + -0.5f, position.y + -0.5f, position.z + -0.5f,
						uv_x_1, uv_y_1,
						0.5f, 0.0f, 0.0f, 0.5f };
			vertices[usedVertices + 1] = ColorVertex{ position.x + 0.5f, position.y + -0.5f, position.z + -0.5f,
							uv_x_2, uv_y_1,
							0.0f, 0.0f, 0.5f, 0.5f };
			vertices[usedVertices + 2] = ColorVertex{ position.x + 0.5f, position.y + -0.5f, position.z + 0.5f,
							uv_x_2, uv_y_2,
							0.0f, 0.5f, 0.5f, 0.5f };
			vertices[usedVertices + 3] = ColorVertex{ position.x + -0.5f, position.y + -0.5f, position.z + 0.5f,
							uv_x_1, uv_y_2,
							0.0, 0.5f, 0.0f, 0.5f };
			break;
		}
		case 5: {
			vertices[usedVertices] = ColorVertex{ position.x + -0.5f, position.y + 0.5f, position.z + 0.5f,
						uv_x_1, uv_y_1,
						0.5f, 0.0f, 0.0f, 0.5f };
			vertices[usedVertices + 1] = ColorVertex{ position.x + 0.5f, position.y + 0.5f, position.z + 0.5f,
							uv_x_2, uv_y_1,
							0.0f, 0.0f, 0.5f, 0.5f };
			vertices[usedVertices + 2] = ColorVertex{ position.x + 0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_2, uv_y_2,
							0.0f, 0.5f, 0.5f, 0.5f };
			vertices[usedVertices + 3] = ColorVertex{ position.x + -0.5f, position.y + 0.5f, position.z + -0.5f,
							uv_x_1, uv_y_2,
							0.0, 0.5f, 0.0f, 0.5f };
			break;
		}

		default:
			break;
		}
		usedVertices += 4;
		
	}

	void add_Block(glm::vec3 position) {
		addPlane(glm::vec3(0, 0, 0) + position);
		addPlane(glm::vec3(0, 0, 0) + position, 1);
		addPlane(glm::vec3(0, 0, 0) + position, 2);
		addPlane(glm::vec3(0, 0, 0) + position, 3);
		addPlane(glm::vec3(0, 0, 0) + position, 4);
		addPlane(glm::vec3(0, 0, 0) + position, 5);
	}
	void add_grass_Block(glm::vec3 position) {
		addPlane(glm::vec3(0, 0, 0) + position, 0, 2, 0);
		addPlane(glm::vec3(0, 0, 0) + position, 1, 2, 0);
		addPlane(glm::vec3(0, 0, 0) + position, 2, 2, 0);
		addPlane(glm::vec3(0, 0, 0) + position, 3, 2, 0);
		addPlane(glm::vec3(0, 0, 0) + position, 4, 1, 0);
		addPlane(glm::vec3(0, 0, 0) + position, 5, 3, 0);
	}

	void clearMesh() {

	}

private:
	uint32 numMaxVertices = 16*16*16*24;
	ColorVertex *vertices =  new ColorVertex[16 * 16 * 16 * 24];
	uint32 usedVertices = 0;

	uint32 numMaxIndices = 16 * 16 * 16 * 36;
	uint32 *indices = new uint32[16 * 16 * 16 * 36];
	uint32 usedIndices = 0;

	int textureAtlasSize = 4;
	float tex_factor = 1.0f / textureAtlasSize;
};

class Plane {
public:
	Plane() {}
	uint32 getNumIndices() {
		return numIndices;
	}
	uint32 getNumVertices() {
		return numVertices;
	}
	ColorVertex* getVertices() {
		return vertices;
	}
	uint32* getIndices() {
		return indices;
	}
private:
	uint32 numVertices = 4;
	ColorVertex vertices[4] = {
		ColorVertex{-1.0f, -1.0f, -1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, -1.0f, -1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, -1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		ColorVertex{-1.0f, 1.0f, -1.0f,
		0.0f, 1.0f,
		0.0, 1.0f, 0.0f, 1.0f},
	};
	uint32 numIndices = 6;
	uint32 indices[6] = {
		0, 1, 2,//front
		0, 2, 3
	};
};

class Box {
public:
	Box(const char* VertexShaderFilename, const char* fragmentShaderFilename,  const char* TextureFilename)  {
		shader.initialize(VertexShaderFilename, fragmentShaderFilename);
		PositionUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_pos");
		if (PositionUniformLocation != -1) {
			//std::cout << "cloudn't create PositionUniformLocation for selection box" << std::endl;
			GLCALL(glUniform3f(PositionUniformLocation, 1.0f, 0.0f, 1.0f));
		}
		modelViewProjMatrixLocation = GLCALL(glGetUniformLocation(shader.getShaderId(), "u_modelViewProj"));
		texture.load(TextureFilename);
		glBindTexture(1, texture.get_textureId());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(1, 0);
	}
	int getNumIndices() {
		return numIndices;
	}
	int getNumVertices() {
		return numVertices;
	}
	ColorVertex* getVertices() {
		return vertices;
	}
	uint32* getIndices() {
		return indices;
	}
	void render(int x, int y, int z, const GLfloat* modelViewProj){
		shader.bind();
		GLCALL(glUniform3f(PositionUniformLocation, x, y, z));
		vertex_buffer.bind();
		index_buffer.bind();
		GLCALL(glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, modelViewProj));
		GLCALL(glActiveTexture(GL_TEXTURE0));
		GLCALL(glBindTexture(GL_TEXTURE_2D, texture.get_textureId()));
		glDisable(GL_CULL_FACE);
		GLCALL(glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0));
		glEnable(GL_CULL_FACE);
		index_buffer.unbind();
		vertex_buffer.unbind();
		shader.unbind();
	}

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
	const char* TextureFilename) : Box(VertexShaderFilename, fragmentShaderFilename, TextureFilename){
		vertex_buffer.update(vertices, numVertices);
		index_buffer.update(indices, numIndices, sizeof(indices[0]));
	}

	void render(float x, float y, float z, const GLfloat* modelViewProj){
		glDisable(GL_DEPTH_TEST);
		shader.bind();
		GLCALL(glUniform3f(PositionUniformLocation, x, y, z));
		vertex_buffer.bind();
		index_buffer.bind();
		GLCALL(glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, modelViewProj));
		GLCALL(glActiveTexture(GL_TEXTURE0));
		GLCALL(glBindTexture(GL_TEXTURE_2D, texture.get_textureId()));
		glDisable(GL_CULL_FACE);
		GLCALL(glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0));
		glEnable(GL_CULL_FACE);
		index_buffer.unbind();
		vertex_buffer.unbind();
		shader.unbind();
		glEnable(GL_DEPTH_TEST);
	}
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

	Quad(){
		glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
	}
	void draw(){
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};
/* struct UIQuad : Quad{
	UIQuad():Quad(){

	}

}; */

struct UImesh{
	std::vector<Vertex> vertices;
	uint32 usedVertices = 0;

	std::vector<uint32> indices;
	uint32 usedIndices = 0;

	int textureAtlasSize = 4;
	float tex_factor = 1.0f / textureAtlasSize;
	ChunkVertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;

	float aspect_ratio;
	float inv_aspect_ratio;
/////////////////////////////////////////////////////
	UImesh() {
		vertexBuffer = *new ChunkVertexBuffer();
		indexBuffer = *new IndexBuffer();

		reserveVertices(100*4);
		reserveIndices(100 * 6);

		aspect_ratio = (float)sdl_handler.getHeight() / (float)sdl_handler.getWidth();
		
	}
	void reserveVertices(int numVertices) {
		vertices.reserve(numVertices);
	}
	void reserveIndices(int numIndices) {
		indices.reserve(numIndices);
	}
	~UImesh() {
		std::vector<Vertex>().swap(vertices);
		std::vector<uint32>().swap(indices);
		vertexBuffer.~ChunkVertexBuffer();
		indexBuffer.~IndexBuffer();
	}
	uint32 getNumIndices() {return usedIndices;}
	uint32 getNumVertices() {return usedVertices;}
	Vertex* getVertices() {return vertices.data();}
	uint32* getIndices() {return indices.data();}

	void update(){
		//Vertex* vertices = getVertices();
		//uint32* indices = getIndices();
		indexBuffer.update(getIndices(), usedIndices, sizeof(getIndices()[0])); // fix sizeof
		vertexBuffer.update(&getVertices()[0], usedVertices);
		//std::cout << "Updating UI: indices:" << usedIndices << std::endl;
	}
	void addQuad(glm::vec3 position, int rotation = 0, int tex_x = 0, int tex_y = 0, float scale = 1) {

		float uv_x_1 = tex_x * tex_factor;
		float uv_x_2 = tex_x * tex_factor + tex_factor;
		float uv_y_1 = 1.0f - tex_y * tex_factor - tex_factor;
		float uv_y_2 = 1.0f - tex_y * tex_factor;

		indices.push_back(usedVertices + 0);
		indices.push_back(usedVertices + 1);
		indices.push_back(usedVertices + 2);
		indices.push_back(usedVertices + 0);
		indices.push_back(usedVertices + 2);
		indices.push_back(usedVertices + 3);
		usedIndices += 6;
		vertices.push_back(Vertex{ (position.x - 0.5f*scale)*aspect_ratio, position.y - 0.5f*scale, position.z + 0.5f,
						uv_x_1, uv_y_1,
						1.0});
		vertices.push_back(Vertex{ (position.x + 0.5f*scale)*aspect_ratio, position.y - 0.5f*scale, position.z + 0.5f,
						uv_x_2, uv_y_1,
						1.0 });
		vertices.push_back(Vertex{ (position.x + 0.5f*scale)*aspect_ratio, position.y + 0.5f*scale, position.z + 0.5f,
						uv_x_2, uv_y_2,
						1.0 });
		vertices.push_back(Vertex{ (position.x - 0.5f*scale)*aspect_ratio, position.y + 0.5f*scale, position.z + 0.5f,
						uv_x_1, uv_y_2,
						1.0 });
		usedVertices += 4; 
	}

	void clearMesh() {
		vertices.clear();
		indices.clear();
		usedIndices = 0;
		usedVertices = 0;
	}

	void render(GLuint textureId) {  // disable Depth testing
		//makes fully transparent images work
		/* glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc (GL_GREATER, 0.1);
		glEnable(GL_ALPHA_TEST); */

		glDisable(GL_DEPTH_TEST);
		vertexBuffer.bind();
		indexBuffer.bind();

		//GLCALL(glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, modelViewProj));
		//std::cout << usedIndices << std::endl;
		GLCALL(glActiveTexture(GL_TEXTURE0));
		GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
		GLCALL(glDrawElements(GL_TRIANGLES, usedIndices, GL_UNSIGNED_INT, 0));

		indexBuffer.unbind();
		vertexBuffer.unbind();
		glEnable(GL_DEPTH_TEST);
	}
};


glm::vec3 calculate_slot_position(int slot_number){
	return glm::vec3((float)slot_number/6.5 -0.7, -0.9, 0.09);
}




struct ItemBarMesh:UImesh{
	ItemBarMesh():UImesh(){
		glm::vec3 pos;
		for(int i=0; i<9; i++){
			pos = calculate_slot_position(i);
			addQuad(pos, 0, 0, 0, 0.15);

		}
		//pos = glm::vec3((float)5/6.5 -0.7, -0.9, 0.09);
		//addQuad(pos, 0, 0, 0, 0.15);
		update();
	}
};
struct InventoryMesh{
	ItemBarMesh itemBar;
	UImesh slotSelector;
	UImesh items;
	int last_selected_slot=5;
	InventoryMesh(){
		/* glm::vec3 pos = calculate_slot_position(1);
		items.addQuad(pos, 0, 1, 0, 0.11);
		items.update(); */
	}

	void update(){
		itemBar.update();
		//glm::vec3 pos = glm::vec3((float)5/6.5 -0.7, -0.9, 0.09);
		//slotSelector.addQuad(pos, 0, 1, 0, 0.16);
		setSlot(last_selected_slot);
		slotSelector.update();
		items.update();
	}
	

	void setSlot(int slot_number){ // IMPORTANT: if the slot number comes from the player one smaller (number -1), so this function ranges from 0 to 8
		if (slot_number != last_selected_slot && slot_number>=0 && slot_number<9){
			//std::cout << "test: SetSlot" << std::endl;
			slotSelector.clearMesh();
			glm::vec3 pos = calculate_slot_position(slot_number);
			slotSelector.addQuad(pos, 0, 1, 0, 0.16);
			slotSelector.update();
			last_selected_slot = slot_number;
		}
	}
	

	void render(GLuint UItextureId, GLuint BlocktextureId){
		itemBar.render(UItextureId);
		slotSelector.render(UItextureId);
		items.render(BlocktextureId);
	}

};





class ScreenQuad{
	Vertex2D vertices[4] = {Vertex2D{0.0f, 1.0f, 0.0f, 1.0f},
				   			Vertex2D{1.0f, 1.0f, 1.0f, 1.0f},
				   			Vertex2D{0.0f, 0.0f, 0.0f, 0.0f},
				   			Vertex2D{1.0f, 0.0f, 1.0f, 0.0f}};

	uint32 indices[6] = {0, 1, 2,
						 1, 3, 2};

public:
	uint32 getNumIndices() {return 6;}
	uint32 getNumVertices() {return 4;}
	Vertex2D* getVertices() {return &vertices[0];}
	uint32* getIndices() {return &indices[0];}

};


