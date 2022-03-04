#pragma once
#include <vector>

#include "definitions.hpp"
#include "glm/glm.hpp"

#include "printFunktions.hpp"



class ChunkMesh {
public:
	std::vector<Vertex> obj;
	
	ChunkMesh() {
		
	}
	~ChunkMesh() {
		std::vector<Vertex>().swap(vertices);
		std::vector<uint32>().swap(indices);
	}

	uint32 getNumIndices() {
		return usedIndices;
	}
	uint32 getNumVertices() {
		return usedVertices;
	}
	Vertex* getVertices() {
		return vertices.data();
	}
	
	uint32* getIndices() {
		return indices.data();
	}
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

private:
	uint32 numMaxVertices = 16 * 16 * 16 * 24;
	std::vector<Vertex> vertices;
	uint32 usedVertices = 0;

	uint32 numMaxIndices = 16 * 16 * 16 * 36;
	std::vector<uint32> indices;
	uint32 usedIndices = 0;

	int textureAtlasSize = 4;
	float tex_factor = 1.0f / textureAtlasSize;
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

class Cube_old {
public:
	Cube_old()  {}
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

private:
	uint32 numVertices = 24;
	ColorVertex vertices[24] = {
		ColorVertex{-1.0f, -1.0f, 1.0f, // front
		0.5f, 0.75f,
		1.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, -1.0f, 1.0f,
		0.75f, 0.75f,
		1.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 1.0f,
		0.75f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{-1.0f, 1.0f, 1.0f,
		0.5f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f},

		
		ColorVertex{1.0f, -1.0f, 1.0f, // right
		0.5f, 0.75f,
		0.0f, 0.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, -1.0f, -1.0f,
		0.75f, 0.75f,
		0.0f, 0.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, -1.0,
		0.75f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 1.0,
		0.5f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f},
		
		
		ColorVertex{1.0f, -1.0f, -1.0f, // back
		0.5f, 0.75f,
		0.0f, 1.0f, 1.0f, 1.0f},
		ColorVertex{-1.0f, -1.0f, -1.0,
		0.75f, 0.75f,
		0.0f, 1.0f, 1.0f, 1.0f},
		ColorVertex{-1.0f, 1.0f, -1.0,
		0.75f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, -1.0,
		0.5f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f},
		
		ColorVertex{-1.0f, -1.0f, -1.0f, // left
		0.5f, 0.75f,
		0.0, 1.0f, 0.0f, 1.0f},
		ColorVertex{-1.0f, -1.0f, 1.0f,
		0.75f, 0.75f,
		0.0, 1.0f, 0.0f, 1.0f},
		ColorVertex{-1.0f, 1.0f, 1.0f,
		0.75f, 1.0f,
		0.0, 1.0f, 0.0f, 1.0f},
		ColorVertex{-1.0f, 1.0f, -1.0,
		0.5f, 1.0f,
		0.0, 1.0f, 0.0f, 1.0f},
		
		ColorVertex{-1.0f, -1.0f, -1.0f, // buttom
		0.25f, 0.75f,
		0.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, -1.0f, -1.0f,
		0.5f, 0.75f,
		0.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{1.0f, -1.0f, 1.0f,
		0.5f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f},
		ColorVertex{-1.0f, -1.0f, 1.0f,
		0.25f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f},
		
		ColorVertex{-1.0f, 1.0f, 1.0f, // top
		0.75f, 0.75f,
		1.0, 1.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, 1.0f,
		1.0f, 0.75f,
		1.0, 1.0f, 1.0f, 1.0f},
		ColorVertex{1.0f, 1.0f, -1.0,
		1.0f, 1.0f,
		1.0, 1.0f, 1.0f, 1.0f},
		ColorVertex{-1.0f, 1.0f, -1.0,
		0.75f, 1.0f,
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
	
};

