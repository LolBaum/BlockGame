
#include "object_models.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

/////////////////////////////////////////////////////////////////////
// Mesh

Mesh::Mesh() {
    vertexBuffer = *new ChunkVertexBuffer();
    indexBuffer = *new IndexBuffer();
}
Mesh::~Mesh() {
    std::vector<Vertex>().swap(vertices);
    std::vector<uint32>().swap(indices);
    vertexBuffer.~ChunkVertexBuffer();
    indexBuffer.~IndexBuffer();
}
uint32 Mesh::getNumIndices() {return usedIndices;}
uint32 Mesh::getNumVertices() {return usedVertices;}
Vertex* Mesh::getVertices() {return vertices.data();}
uint32* Mesh::getIndices() {return indices.data();}

void Mesh::reserveVertices(int numVertices) {
    vertices.reserve(numVertices);
}
void Mesh::reserveIndices(int numIndices) {
    indices.reserve(numIndices);
}

void Mesh::initialize(){}

void Mesh::render(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId) {
    vertexBuffer.bind();
    indexBuffer.bind();
    GLCALL(glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, modelViewProj));
    GLCALL(glActiveTexture(GL_TEXTURE0));
    GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
    GLCALL(glDrawElements(GL_TRIANGLES, usedIndices, GL_UNSIGNED_INT, 0));
    indexBuffer.unbind();
    vertexBuffer.unbind();
}

void Mesh::update(){
    indexBuffer.update(getIndices(), usedIndices, sizeof(getIndices()[0]));
    vertexBuffer.update(&getVertices()[0], usedVertices);
}

void Mesh::clearMesh() {
    vertices.clear();
    indices.clear();
    usedIndices = 0;
    usedVertices = 0;
}


/////////////////////////////////////////////////////////////////////
// ChunkMesh

ChunkMesh::ChunkMesh():Mesh() {}
ChunkMesh::~ChunkMesh() {}

void ChunkMesh::reserveFaces(int numFaces) {
    reserveVertices(numFaces*4);
    reserveIndices(numFaces * 6);
    // could return Int as Warning
}

// rotation: front-0, right-1, back-2, left-3, buttom-4, top-5,
void ChunkMesh::addPlane(glm::vec3 position, int rotation, int tex_x, int tex_y, int size) {

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

void ChunkMesh::addPlane_basic_lighting(glm::vec3 position, BlockDirection rotation, int tex_x, int tex_y, int size,
                                        int face_rot) {

    float uv_x_0, uv_x_1, uv_x_2, uv_x_3;
    float uv_y_0, uv_y_1, uv_y_2, uv_y_3;


    if(face_rot == 0){
        uv_x_0 = tex_x * tex_factor;
        uv_x_1 = tex_x * tex_factor + tex_factor;
        uv_x_2 = uv_x_1;
        uv_x_3 = uv_x_0;
        uv_y_0 = 1.0f - tex_y * tex_factor - tex_factor;
        uv_y_3 = 1.0f - tex_y * tex_factor;
        uv_y_1 = uv_y_0;
        uv_y_2 = uv_y_3;
    }else if(face_rot == 1){
        uv_x_0 = tex_x * tex_factor + tex_factor;
        uv_x_1 = uv_x_0;
        uv_x_2 = tex_x * tex_factor;
        uv_x_3 = uv_x_2;
        uv_y_0 = 1.0f - tex_y * tex_factor - tex_factor;
        uv_y_3 = uv_y_0;
        uv_y_1 = 1.0f - tex_y * tex_factor;
        uv_y_2 = uv_y_1;
    } else{
        uv_x_0 = tex_x * tex_factor;
        uv_x_1 = tex_x * tex_factor + tex_factor;
        uv_x_2 = uv_x_1;
        uv_x_3 = uv_x_0;
        uv_y_0 = 1.0f - tex_y * tex_factor - tex_factor;
        uv_y_3 = 1.0f - tex_y * tex_factor;
        uv_y_1 = uv_y_0;
        uv_y_2 = uv_y_3;
        std::cout << "ChunkMesh::addPlane_basic_lighting face_rot undefined value: " << face_rot << std::endl;
    }

    indices.push_back(usedVertices + 0);
    indices.push_back(usedVertices + 1);
    indices.push_back(usedVertices + 2);
    indices.push_back(usedVertices + 0);
    indices.push_back(usedVertices + 2);
    indices.push_back(usedVertices + 3);
    usedIndices += 6;

    switch (rotation)
    {
    case Z_positive: {

        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 1.0f,
                    uv_x_0, uv_y_0,
                    0.7 });
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 1.0f,
                        uv_x_1, uv_y_1,
                        0.7 });
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 1.0f,
                        uv_x_2, uv_y_2,
                        0.7 });
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 1.0f,
                        uv_x_3, uv_y_3,
                        0.7 });
        break;
    }
    case X_positive: {
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 1.0f,
                    uv_x_0, uv_y_0,
                    0.8 });
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 0.0f,
                        uv_x_1, uv_y_1,
                        0.8 });
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 0.0f,
                        uv_x_2, uv_y_2,
                        0.8 });
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 1.0f,
                        uv_x_3, uv_y_3,
                        0.8 });
        break;
    }
    case Z_negative: {
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 0.0f,
                    uv_x_0, uv_y_0,
                    0.7 });
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 0.0f,
                        uv_x_1, uv_y_1,
                        0.7 });
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 0.0f,
                        uv_x_2, uv_y_2,
                        0.7 });
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 0.0f,
                        uv_x_3, uv_y_3,
                        0.7 });
        break;
    }
    case X_negative: {
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 0.0f,
                    uv_x_0, uv_y_0,
                    0.8 });
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 1.0f,
                        uv_x_1, uv_y_1,
                        0.8 });
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 1.0f,
                        uv_x_2, uv_y_2,
                        0.8 });
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 0.0f,
                        uv_x_3, uv_y_3,
                        0.8 });
        break;
    }
    case Y_negative: {
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 0.0f,
                    uv_x_0, uv_y_0,
                    0.6 });
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 0.0f,
                        uv_x_1, uv_y_1,
                        0.6 });
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 1.0f,
                        uv_x_2, uv_y_2,
                        0.6 });
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 1.0f,
                        uv_x_3, uv_y_3,
                        0.6 });
        break;
    }
    case Y_positive: {
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 1.0f,
                    uv_x_0, uv_y_0,
                    1.0 });
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 1.0f,
                        uv_x_1, uv_y_1,
                        1.0 });
        vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 0.0f,
                        uv_x_2, uv_y_2,
                        1.0 });
        vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 0.0f,
                        uv_x_3, uv_y_3,
                        1.0 });
        break;
    }

    default:
        break;
    }
    usedVertices += 4;
}

void ChunkMesh::addPlane_basic_lighting_with_offset(glm::vec3 position, int rotation, int tex_x, int tex_y, float size) {

    float uv_x_1 = tex_x * tex_factor;
    float uv_x_2 = tex_x * tex_factor + tex_factor;
    float uv_y_1 = 1.0f - tex_y * tex_factor - tex_factor;
    float uv_y_2 = 1.0f - tex_y * tex_factor;

    float upper_offset = size;
    float lower_offset = 1-size;

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

            vertices.push_back(Vertex{ position.x + lower_offset, position.y + lower_offset, position.z + size,
                                       uv_x_1, uv_y_1,
                                       0.7 });
            vertices.push_back(Vertex{ position.x + size, position.y + lower_offset, position.z + size,
                                       uv_x_2, uv_y_1,
                                       0.7 });
            vertices.push_back(Vertex{ position.x + size, position.y + size, position.z + size,
                                       uv_x_2, uv_y_2,
                                       0.7 });
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + size, position.z + size,
                                       uv_x_1, uv_y_2,
                                       0.7 });
            break;
        }
        case 1: {
            vertices.push_back(Vertex{ position.x + size, position.y + lower_offset, position.z + size,
                                       uv_x_1, uv_y_1,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + size, position.y + lower_offset, position.z + lower_offset,
                                       uv_x_2, uv_y_1,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + size, position.y + size, position.z + lower_offset,
                                       uv_x_2, uv_y_2,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + size, position.y + size, position.z + size,
                                       uv_x_1, uv_y_2,
                                       0.8 });
            break;
        }
        case 2: {
            vertices.push_back(Vertex{ position.x + size, position.y + lower_offset, position.z + lower_offset,
                                       uv_x_1, uv_y_1,
                                       0.7 });
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + lower_offset, position.z + lower_offset,
                                       uv_x_2, uv_y_1,
                                       0.7 });
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + size, position.z + lower_offset,
                                       uv_x_2, uv_y_2,
                                       0.7 });
            vertices.push_back(Vertex{ position.x + size, position.y + size, position.z + lower_offset,
                                       uv_x_1, uv_y_2,
                                       0.7 });
            break;
        }
        case 3: {
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + lower_offset, position.z + lower_offset,
                                       uv_x_1, uv_y_1,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + lower_offset, position.z + size,
                                       uv_x_2, uv_y_1,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + size, position.z + size,
                                       uv_x_2, uv_y_2,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + size, position.z + lower_offset,
                                       uv_x_1, uv_y_2,
                                       0.8 });
            break;
        }
        case 4: {
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + lower_offset, position.z + lower_offset,
                                       uv_x_1, uv_y_1,
                                       0.6 });
            vertices.push_back(Vertex{ position.x + size, position.y + lower_offset, position.z + lower_offset,
                                       uv_x_2, uv_y_1,
                                       0.6 });
            vertices.push_back(Vertex{ position.x + size, position.y + lower_offset, position.z + size,
                                       uv_x_2, uv_y_2,
                                       0.6 });
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + lower_offset, position.z + size,
                                       uv_x_1, uv_y_2,
                                       0.6 });
            break;
        }
        case 5: {
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + size, position.z + size,
                                       uv_x_1, uv_y_1,
                                       1.0 });
            vertices.push_back(Vertex{ position.x + size, position.y + size, position.z + size,
                                       uv_x_2, uv_y_1,
                                       1.0 });
            vertices.push_back(Vertex{ position.x + size, position.y + size, position.z + lower_offset,
                                       uv_x_2, uv_y_2,
                                       1.0 });
            vertices.push_back(Vertex{ position.x + lower_offset, position.y + size, position.z + lower_offset,
                                       uv_x_1, uv_y_2,
                                       1.0 });
            break;
        }

        default:
            break;
    }
    usedVertices += 4;
}

void ChunkMesh::add_Block(glm::vec3 position, int tex_x, int tex_y) {
    addPlane(glm::vec3(0, 0, 0) + position);
    addPlane(glm::vec3(0, 0, 0) + position, 1, tex_x, tex_y);
    addPlane(glm::vec3(0, 0, 0) + position, 2, tex_x, tex_y);
    addPlane(glm::vec3(0, 0, 0) + position, 3, tex_x, tex_y);
    addPlane(glm::vec3(0, 0, 0) + position, 4, tex_x, tex_y);
    addPlane(glm::vec3(0, 0, 0) + position, 5, tex_x, tex_y);
	}

void ChunkMesh::add_Block_with_offset(glm::vec3 position, int tex_x, int tex_y, float size) {
    addPlane_basic_lighting_with_offset(glm::vec3(0, 0, 0) + position, 0, tex_x, tex_y, size);
    addPlane_basic_lighting_with_offset(glm::vec3(0, 0, 0) + position, 1, tex_x, tex_y, size);
    addPlane_basic_lighting_with_offset(glm::vec3(0, 0, 0) + position, 2, tex_x, tex_y, size);
    addPlane_basic_lighting_with_offset(glm::vec3(0, 0, 0) + position, 3, tex_x, tex_y, size);
    addPlane_basic_lighting_with_offset(glm::vec3(0, 0, 0) + position, 4, tex_x, tex_y, size);
    addPlane_basic_lighting_with_offset(glm::vec3(0, 0, 0) + position, 5, tex_x, tex_y, size);
}

void ChunkMesh::diagonal_Plane(glm::vec3 position, int tex_x, int tex_y, float size, int c) {
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

    switch (c) {
        case 0:

            vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 0.0f,
                                       uv_x_1, uv_y_1,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 0.0f,
                                       uv_x_1, uv_y_2,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 1.0f,
                                       uv_x_2, uv_y_2,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 1.0f,
                                       uv_x_2, uv_y_1,
                                       0.8 });
            break;
        case 1:
            vertices.push_back(Vertex{ position.x + 1.0f, position.y + 0.0f, position.z + 0.0f,
                                       uv_x_1, uv_y_1,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + 1.0f, position.y + 1.0f, position.z + 0.0f,
                                       uv_x_1, uv_y_2,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + 0.0f, position.y + 1.0f, position.z + 1.0f,
                                       uv_x_2, uv_y_2,
                                       0.8 });
            vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, position.z + 1.0f,
                                       uv_x_2, uv_y_1,
                                       0.8 });
            break;
    }


    usedVertices += 4;
}


void ChunkMesh::add_cross(glm::vec3 position, int tex_x, int tex_y, float size) {
    diagonal_Plane(position, tex_x, tex_y, size, 0);
    diagonal_Plane(position, tex_x, tex_y, size, 1);
}



/////////////////////////////////////////////////////////////////////
// Box


Box::Box(const char* VertexShaderFilename, const char* fragmentShaderFilename,  const char* TextureFilename)  {
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
int Box::getNumIndices() {
    return numIndices;
}
int Box::getNumVertices() {
    return numVertices;
}
ColorVertex* Box::getVertices() {
    return vertices;
}
uint32* Box::getIndices() {
    return indices;
}
void Box::render(int x, int y, int z, const GLfloat* modelViewProj){
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


/////////////////////////////////////////////////////////////////////
// Skybox


Skybox::Skybox(const char* VertexShaderFilename, const char* fragmentShaderFilename,  
const char* TextureFilename) : Box(VertexShaderFilename, fragmentShaderFilename, TextureFilename){
    vertex_buffer.update(vertices, numVertices);
    index_buffer.update(indices, numIndices, sizeof(indices[0]));
}

void Skybox::render(float x, float y, float z, const GLfloat* modelViewProj){
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




/////////////////////////////////////////////////////////////////////
// Quad

Quad::Quad(){
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
void Quad::draw(){
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}


/////////////////////////////////////////////////////////////////////////
// SkyQuad


SkyQuad::SkyQuad(const char* VertexShaderFilename, const char* fragmentShaderFilename):Quad(){
    shader.initialize(VertexShaderFilename, fragmentShaderFilename);
    lookAt_uniformLocation = glGetUniformLocation(shader.getShaderId(), "u_LookAt");
    InvProjection_uniformLocation = glGetUniformLocation(shader.getShaderId(), "InvProjection");
    InvView_uniformLocation = glGetUniformLocation(shader.getShaderId(), "InvView");
}
void SkyQuad::render(glm::vec3 lookAt, const GLfloat* InvProjection, const GLfloat* InvView){
    shader.bind();
    GLCALL(glUniform3f(lookAt_uniformLocation, lookAt.x, lookAt.y, lookAt.z));
    GLCALL(glUniformMatrix4fv(InvProjection_uniformLocation, 1, GL_FALSE, InvProjection));
    GLCALL(glUniformMatrix4fv(InvView_uniformLocation, 1, GL_FALSE, InvView));

    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    
    shader.unbind();
}




/////////////////////////////////////////////////////////////////////
// UImesh
glm::vec3 calculate_slot_position(int slot_number){
    float margin_y = 16;
    float size = 80;
    float padding_between = 4;
    float start_left = ((float)SDL_handler::getWidth() - (size*9 + padding_between*8)) / 2;
	return glm::vec3(start_left + size*(slot_number+1) + padding_between*(slot_number),
                     margin_y, 0.0);
}


UImesh::UImesh(int texture_size) {
    vertexBuffer = *new ChunkVertexBuffer();
    indexBuffer = *new IndexBuffer();

    textureAtlasSize = texture_size;
    tex_factor = 1.0f / textureAtlasSize;

    reserveVertices(100*4);
    reserveIndices(100 * 6);

    aspect_ratio = (float)SDL_handler::getHeight() / (float)SDL_handler::getWidth();
}

void UImesh::reserveVertices(int numVertices) {
    vertices.reserve(numVertices);
}

void UImesh::reserveIndices(int numIndices) {
    indices.reserve(numIndices);
}

UImesh::~UImesh() {
    std::vector<Vertex>().swap(vertices);
    std::vector<uint32>().swap(indices);
    vertexBuffer.~ChunkVertexBuffer();
    indexBuffer.~IndexBuffer();
}

uint32 UImesh::getNumIndices() {return usedIndices;}
uint32 UImesh::getNumVertices() {return usedVertices;}
Vertex* UImesh::getVertices() {return vertices.data();}
uint32* UImesh::getIndices() {return indices.data();}

void UImesh::update(){
    indexBuffer.update(getIndices(), usedIndices, sizeof(getIndices()[0])); // fix sizeof
    vertexBuffer.update(&getVertices()[0], usedVertices);

}
void UImesh::addQuad(glm::vec3 position, int rotation, int tex_x, int tex_y, float scale) {

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
    vertices.push_back(Vertex{ (position.x), position.y, 0.0f,
                    uv_x_1, uv_y_1,
                    1.0});
    vertices.push_back(Vertex{ (position.x + scale), position.y, 0.0f,
                    uv_x_2, uv_y_1,
                    1.0 });
    vertices.push_back(Vertex{ (position.x + scale), position.y + scale, 0.0f,
                    uv_x_2, uv_y_2,
                    1.0 });
    vertices.push_back(Vertex{ (position.x), position.y + scale, 0.0f,
                    uv_x_1, uv_y_2,
                    1.0 });
    usedVertices += 4; 
}

void UImesh::clearMesh() {
    vertices.clear();
    indices.clear();
    usedIndices = 0;
    usedVertices = 0;
}

void UImesh::render(GLuint textureId) {  // disable Depth testing
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



/////////////////////////////////////////////////////////////////////
// ItemBarMesh


ItemBarMesh::ItemBarMesh(int texture_size) : UImesh(texture_size){
    glm::vec3 pos;
    for(int i=0; i<9; i++){
        pos = calculate_slot_position(i);
        addQuad(pos, 0, 0, 0, 80);

    }
    //pos = glm::vec3((float)5/6.5 -0.7, -0.9, 0.09);
    //addQuad(pos, 0, 0, 0, 0.15);
    update();
}



/////////////////////////////////////////////////////////////////////
// InventoryMesh


InventoryMesh::InventoryMesh(){}

void InventoryMesh::update(){
    itemBar.update();
    //glm::vec3 pos = glm::vec3((float)5/6.5 -0.7, -0.9, 0.09);
    //slotSelector.addQuad(pos, 0, 1, 0, 0.16);
    setSlot(last_selected_slot);
    slotSelector.update();
    items.update();
}

void InventoryMesh::setSlot(int slot_number){ // IMPORTANT: if the slot number comes from the player one smaller (number -1), so this function ranges from 0 to 8
    if (slot_number != last_selected_slot && slot_number>=0 && slot_number<9){
        //std::cout << "test: SetSlot" << std::endl;
        slotSelector.clearMesh();
        glm::vec3 pos = calculate_slot_position(slot_number);
        slotSelector.addQuad(pos, 0, 1, 0, 80);
        slotSelector.update();
        last_selected_slot = slot_number;
    }
}

void InventoryMesh::render(GLuint UItextureId, GLuint BlocktextureId){
    itemBar.render(UItextureId);
    slotSelector.render(UItextureId);
    items.render(BlocktextureId);
}


/////////////////////////////////////////////////////////////////////
// TextMesh



TextMesh::TextMesh(float sx, float sy):Mesh(){
    sizeX = sx;
    sizeY = sy;
}
TextMesh::~TextMesh(){}

void TextMesh::reserveCharacters(int numChars){
    reserveVertices(numChars * 4);
    reserveIndices(numChars * 6);
}

void TextMesh::addCharacter(glm::vec2 position, int rotation, float x, float y, float scale_x, float scale_y,
                            float brightness){

    float uv_x_1 = x * sizeX;
    float uv_x_2 = (x + 1) * sizeX;
    float uv_y_1 = 1.0 - (y + 1) * sizeY;
    float uv_y_2 = 1.0 -  y      * sizeY;

    indices.push_back(usedVertices + 0);
    indices.push_back(usedVertices + 1);
    indices.push_back(usedVertices + 2);
    indices.push_back(usedVertices + 0);
    indices.push_back(usedVertices + 2);
    indices.push_back(usedVertices + 3);
    usedIndices += 6;


    vertices.push_back(Vertex{ position.x + 0.0f, position.y + 0.0f, 0.0f,
                               uv_x_1, uv_y_1,
                               brightness});
    vertices.push_back(Vertex{ position.x + scale_x, position.y + 0.0f, 0.0f,
                               uv_x_2, uv_y_1,
                               brightness });
    vertices.push_back(Vertex{ position.x + scale_x, position.y + scale_y, 0.0f,
                               uv_x_2, uv_y_2,
                               brightness});
    vertices.push_back(Vertex{ position.x + 0.0f, position.y + scale_y, 0.0f,
                               uv_x_1, uv_y_2,
                               brightness});

    usedVertices += 4;
}

void TextMesh::render(GLuint textureId) {  // disable Depth testing

    glDisable(GL_DEPTH_TEST);
    //glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE);
    vertexBuffer.bind();
    indexBuffer.bind();

    //std::cout << usedIndices << std::endl;
    GLCALL(glActiveTexture(GL_TEXTURE0));
    GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
    GLCALL(glDrawElements(GL_TRIANGLES, usedIndices, GL_UNSIGNED_INT, 0));

    indexBuffer.unbind();
    vertexBuffer.unbind();
    glEnable(GL_DEPTH_TEST);
}
