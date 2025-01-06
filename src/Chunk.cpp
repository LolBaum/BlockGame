#include "Chunk.hpp"
#include "SuperChunk.hpp"
#include "object_models.hpp"
#include "util_funcs.hpp"
#include "printFunktions.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>


Chunk::Chunk(glm::vec3 position) {
    pos = position;
    id = map_3d_to_1d(pos);
    numBlocks = 0;
    changed = true;

//        calcChunkCoords(pos, &px, &py, &pz);
        //std::cout << vec3_toString(pos) << std::endl;
    px = pos.x;
    py = pos.y;
    pz = pos.z;
    //printf("%d %d %d \n", px,py,pz);

    // vertexBuffer = *new ChunkVertexBuffer();
    // indexBuffer = *new IndexBuffer();
}

Chunk::~Chunk(){
    //std::cout << "Called Destructor for Chunk: " << pos.x << ", " << pos.y << ", " << pos.z << ", " << std::endl;

    // vertexBuffer.~ChunkVertexBuffer();
    // indexBuffer.~IndexBuffer();

    mesh.~ChunkMesh();
}

void Chunk::clearChunk() {
    //std::cout << "cleared Chunk: " << vec3_toString(pos) << std::endl;
    pos = glm::vec3(-1, -1, -1);
    px=-1;
    py=-1;
    pz=-1;

    id = 0;
    for (int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            for (int z = 0; z < CZ; z++) {
                blocks[x][y][z].setId(0);
            }
        }
    }
    changed = true;
    updateMesh();
    
}

void Chunk::setBlock(int x, int y, int z, int type, BlockRotation rot) {
    if (0 <= x && x <= 15 && 0 <= y && y <= 15 && 0 <= z && z <= 15) {
        blocks[x][y][z].setId(type);
        blocks[x][y][z].setRot(rot);
        changed = true;
        //changed_since_loading = true;
    }
} //add changes
void Chunk::setBlock(glm::vec3 pos, int type, BlockRotation rot) {
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    if (0 <= x && x <= 15 && 0 <= y && y <= 15 && 0 <= z && z <= 15) {
        blocks[x][y][z].setId(type);
        blocks[x][y][z].setRot(rot);
        changed = true;
        std::cout << "set Block at: " << vec3_toString(pos) << std::endl;
        changed_since_loading = true;                                           // should only be true i the player changed a block (not worldgen)
        return;
    }
}

int Chunk::getBlockTypeInt(int x, int y, int z) {
    if (0 <= x && x <= 15 && 0 <= y && y <= 15 && 0 <= z && z <= 15) {
        return blocks[x][y][z].getId();
    }
    else {
        return 0;
    }
}
int Chunk::getBlockTypeInt(glm::vec3 pos) {
    int x = pos.x;
    int y = pos.y;
    int z = pos.z;
    if (0 <= x && x <= 15 && 0 <= y && y <= 15 && 0 <= z && z <= 15) {
        return blocks[x][y][z].getId();
    }
    else {
        return 0;
    }
}

BlockType* Chunk::getBlockType(int x, int y, int z) {
    if (0 <= x && x <= 15 && 0 <= y && y <= 15 && 0 <= z && z <= 15) {
        return BlockTypeManager::GetBlockType(blocks[x][y][z].getId());
    }
    else {
        return &BlockTypeManager::Air;
    }
}

Block* Chunk::getBlock(int x, int y, int z) {
    if (0 <= x <= 15 && 0 <= y <= 15 && 0 <= z <= 15) {
        return &blocks[x][y][z];
    }
    return 0; // be carefull, check for NULL when using this function
}

void Chunk::dummyWorldGen(int variant) {
    //std::cout << "filled Chunk: " << vec3_toString(pos) << std::endl;
    switch (variant)
    {
    case 0:
        for (int x = 0; x < CX; x++) {
            for (int y = 0; y < x; y++) {
                for (int z = 0; z < CZ; z++) {
                    blocks[x][y][z].setId(1);
                }
            }
        }
        break;
    case 1:
        for (int x = 0; x < CX; x++) {
            for (int y = 0; y < 1; y++) {
                for (int z = 0; z < CZ; z++) {
                    blocks[x][y][z].setId(1);
                }}}
        break;
    case 2:
        for (int x = 0; x < 2; x++) {
            for (int y = 0; y < CY; y++) {
                for (int z = 0; z < CZ; z++) {
                    blocks[x][y][z].setId(1);
                }
            }
        }
        break;
    case 3:
        for (int x = 0; x < CX; x++) {
            for (int y = 0; y < CY; y++) {
                for (int z = 0; z < CZ; z++) {
                    if (((float)(x * x)/256.0 + (float)(y * y) /256.0 + (float)(z * z) / 256.0) < 1 &&
                        ((float)(x * x) / 256.0 + (float)(y * y) / 256.0 + (float)(z * z) / 256.0) > 0.5) {
                        //std::cout << ((float)(x * x) / 256.0 + (float)(y * y) / 256.0) << std::endl;
                        blocks[x][y][z].setId(1);
                    }
                }
            }
        }
        break;
    case 4:
        for (int x = 0; x < CX; x++) {
            for (int y = 0; y < CY; y++) {
                for (int z = 0; z < CZ; z++) {
                    float x1 = ((x - 8) * (x - 8)) / 128.0;
                    float y1 = ((y - 8) * (y - 8)) / 128.0;
                    float z1 = ((z - 8) * (z - 8)) / 128.0;
                    
                    if ((x1+y1+z1) < 0.3) {
                        
                        blocks[x][y][z].setId(1);
                    }
                }
            }
        }
        break;
    case 5:
        for (int x = 0; x < CX; x++) {
            for (int z = 0; z < CY; z++) {
                int h = (glm::abs(glm::perlin(glm::vec2((x+pos.x)/16.0, (z+pos.z)/16.0))))*16;
                //std::cout << "perlin: " << h << std::endl;
                for (int y = 0; y <= h; y++) {
                    blocks[x][y][z].setId(1);
                }
                
                
            }
        }
        break;
    case 6:
        for (int x = 0; x < CX; x++) {
            for (int y = 0; y < CY; y++) {
                for (int z = 0; z < CY; z++) {
                    float h = glm::abs(glm::perlin(glm::vec3((x + pos.x) / 16.0, (y + pos.y) / 16.0, (z + pos.z) / 16.0)));
                    //std::cout << "perlin: " << h << std::endl;
                    if (h < 0.2) {
                        blocks[x][y][z].setId(1);
                    }
                }
            }

        }
        break;
    default:
        break;
    }
    changed = true;
    updateMesh();
}

void Chunk::updateMesh() {
    //std::cout << "Updating the mesh" << std::endl;
    
    if(changed){

        if (! isChunkEmty(pos)) {
            //std::cout << "Updating mesh: Pos: " << vec3_toString(pos) << std::endl;
        }



        calculateNumFaces();
        mesh.clearMesh();
        mesh.reserveFaces(num_faces);
        mesh_transparent.clearMesh();
        mesh_transparent.reserveFaces(num_faces_transparent);
        mesh_foliage.clearMesh();
        //mesh_foliage.reserveFaces(num_faces_foliage);
        //std::cout << "mesh.reserveFaces("<<NumFaces<<");" << std::endl;
        numBlocks = 0;
        for (int x = 0; x < CX; x++) {
            for (int y = 0; y < CY; y++) {
                for (int z = 0; z < CZ; z++) {
                    int id = blocks[x][y][z].getId();
                    if (id != 0) {
                        numBlocks++;
                        //std::cout << glm::vec3((float32)x, y, z).x << " " <<glm::vec3(x, (float32)y, z).y << std::endl;
                        //std::cout << "adding Block" << std::endl;

                        int cx = pos.x + x;
                        int cy = pos.y + y;
                        int cz = pos.z + z;


                        glm::vec3 dummyPos = glm::vec3(pos.x, pos.y, pos.z);

                        BlockType* bt = getBlockType(x,y,z);
                            switch (bt->get_texture_type()) // ToDo: Get the texture Type from the Blockmanager
                            {
                                // Single Texture
                                case SingleTexture: {
                                    int tex_x = 0;
                                    int tex_y = 0;
                                    bt->get_tex_coords(&tex_x, &tex_y);
                                    if (bt->get_transparency_type()==Solid){

                                        if (getBlockType(x - 1, y, z)->isTransparent()) { // Todo: check for opeaqeness //TODO: Add explanation for this!!!
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 3, tex_x, tex_y);
                                        }
                                        if (getBlockType(x + 1, y, z)->isTransparent()) { // Todo: check for opeaqeness
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 1, tex_x, tex_y);
                                        }
                                        if (getBlockType(x, y - 1, z)->isTransparent()) { // Todo: check for opeaqeness
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 4, tex_x, tex_y);
                                        }
                                        if (getBlockType(x, y + 1, z)->isTransparent()) { // Todo: check for opeaqeness
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 5, tex_x, tex_y);
                                        }
                                        if (getBlockType(x, y, z - 1)->isTransparent()) { // Todo: check for opeaqeness
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 2, tex_x, tex_y);
                                        }
                                        if (getBlockType(x, y, z + 1)->isTransparent()) { // Todo: check for opeaqeness
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 0, tex_x, tex_y);
                                        }
                                    }else if (bt->get_transparency_type()==Transparent) {
                                        mesh_transparent.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 3, tex_x, tex_y);
                                        mesh_transparent.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 1, tex_x, tex_y);
                                        mesh_transparent.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 4, tex_x, tex_y);
                                        mesh_transparent.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 5, tex_x, tex_y);
                                        mesh_transparent.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 2, tex_x, tex_y);
                                        mesh_transparent.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 0, tex_x, tex_y);
                                    }
                                    else if (bt->get_transparency_type()==Foliage){
                                        mesh_foliage.add_cross(glm::vec3(x, y, z) + dummyPos, tex_x, tex_y, 0.9990);
                                        mesh_foliage.add_Block_with_offset(glm::vec3(x, y, z) + dummyPos, tex_x, tex_y, 0.90);
                                    }else if (bt->get_transparency_type()==Glass){
                                        if (getBlockType(x - 1, y, z)->isTransparent()) { // Todo: check for opeaqeness //TODO: Add explanation for this!!!
                                            mesh_foliage.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 3, tex_x, tex_y);
                                        }
                                        if (getBlockType(x + 1, y, z)->isTransparent()) { // Todo: check for opeaqeness
                                            mesh_foliage.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 1, tex_x, tex_y);
                                        }
                                        if (getBlockType(x, y - 1, z)->isTransparent()) { // Todo: check for opeaqeness
                                            mesh_foliage.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 4, tex_x, tex_y);
                                        }
                                        if (getBlockType(x, y + 1, z)->isTransparent()) { // Todo: check for opeaqeness
                                            mesh_foliage.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 5, tex_x, tex_y);
                                        }
                                        if (getBlockType(x, y, z - 1)->isTransparent()) { // Todo: check for opeaqeness
                                            mesh_foliage.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 2, tex_x, tex_y);
                                        }
                                        if (getBlockType(x, y, z + 1)->isTransparent()) { // Todo: check for opeaqeness
                                            mesh_foliage.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 0, tex_x, tex_y);
                                        }
                                    }
                                    else{
                                        mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 3, tex_x, tex_y);
                                        mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 1, tex_x, tex_y);
                                        mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 4, tex_x, tex_y);
                                        mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 5, tex_x, tex_y);
                                        mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 2, tex_x, tex_y);
                                        mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 0, tex_x, tex_y);
                                    }
                                    break;
                                }
                                case MultiTexture: {

                                    if(bt->isRotatable()){

                                        //std::cout << "Mulittexture" << std::endl;
                                        int* order;
                                        int tex_x = 0;
                                        int tex_y = 0;
                                        SpecialBlockTexture* tex = bt->get_multi_texture();

                                        std::cout << "update Chunk Multitexture" << std::endl;

                                        const int order_z_positive[6] = {3, 1, 2, 0, 5, 4};
                                        const int order_z_negative[6] = {3, 1, 0, 2, 4, 5};
                                        const int order_y_positive[6] = {3, 1, 4, 5, 2, 0};
                                        const int order_y_negative[6] = {3, 1, 5, 4, 0, 2};
                                        const int order_x_positive[6] = {4, 5, 3, 1, 2, 0};
                                        const int order_x_negative[6] = {5, 4, 1, 3, 2, 0};// TODO
                                        switch (blocks[x][y][z].getRot()) {
                                            case X_negative:
                                                order = (int*)order_x_negative;
                                                std::cout << "Rotation: X_negative" << std::endl;
                                                break;
                                            case X_positive:
                                                order = (int*)order_x_positive;
                                                std::cout << "Rotation: X_positive" << std::endl;
                                                break;
                                            case Y_negative:
                                                order = (int*)order_y_negative;
                                                std::cout << "Rotation: Y_negative" << std::endl;
                                                break;
                                            case Y_positive:
                                                order = (int*)order_y_positive;
                                                std::cout << "Rotation: Y_positive" << std::endl;
                                                break;
                                            case Z_negative:
                                                order = (int*)order_z_negative;
                                                std::cout << "Rotation: Z_negative" << std::endl;
                                                break;
                                            case Z_positive:
                                                order = (int*)order_z_positive;
                                                std::cout << "Rotation: Z_positive" << std::endl;
                                                break;
                                            default:
                                                order = (int*)order_y_positive;
                                                std::cout << "Chunk::updateMesh Rotation: ERROR" << std::endl;
                                                break;
                                        }

                                        //std::cout << tex << std::endl;
                                        // TODO: add array[6] for plane rotation indices
                                        // TODO: array for each block rotation
                                        if (getBlockType(x - 1, y, z)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_left(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, order[0], tex_x, tex_y); // left
                                        }
                                        if (getBlockType(x + 1, y, z)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_right(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, order[1], tex_x, tex_y); // right
                                        }
                                        if (getBlockType(x, y - 1, z)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_bottom(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, order[2], tex_x, tex_y); // bottom
                                        }
                                        if (getBlockType(x, y + 1, z)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_top(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, order[3], tex_x, tex_y); // top
                                        }
                                        if (getBlockType(x, y, z - 1)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_back(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, order[4], tex_x, tex_y); // back
                                        }
                                        if (getBlockType(x, y, z + 1)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_front(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, order[5], tex_x, tex_y); // front
                                        }
                                        break;
                                    }
                                    else{
                                        //std::cout << "Mulittexture" << std::endl;
                                        int tex_x = 0;
                                        int tex_y = 0;
                                        SpecialBlockTexture* tex = bt->get_multi_texture();
                                        //std::cout << tex << std::endl;
                                        if (getBlockType(x - 1, y, z)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_left(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 3, tex_x, tex_y); // left
                                        }
                                        if (getBlockType(x + 1, y, z)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_right(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 1, tex_x, tex_y); // right
                                        }
                                        if (getBlockType(x, y - 1, z)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_bottom(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 4, tex_x, tex_y); // bottom
                                        }
                                        if (getBlockType(x, y + 1, z)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_top(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 5, tex_x, tex_y); // top
                                        }
                                        if (getBlockType(x, y, z - 1)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_back(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 2, tex_x, tex_y); // back
                                        }
                                        if (getBlockType(x, y, z + 1)->isTransparent()) { // Todo: check for opeaqeness
                                            tex->get_front(&tex_x, &tex_y);
                                            mesh.addPlane_basic_lighting(glm::vec3(x, y, z) + dummyPos, 0, tex_x, tex_y); // front
                                        }
                                        break;
                                    }
                                }
                                case Cross:{
                                    int tex_x = 0;
                                    int tex_y = 0;
                                    bt->get_tex_coords(&tex_x, &tex_y);
                                    mesh_foliage.add_cross(glm::vec3(x, y, z) + dummyPos, tex_x, tex_y, 1);
                                    break;
                                }

                                default:
                                    break;
                                }




                    }
                }
            }
        }

        mesh.update();
        mesh_foliage.update();
        mesh_transparent.update();

        changed = false;
    }
    
}

void Chunk::render(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId) {
    updateMesh();
    mesh.render(modelViewProjMatrixLocation, modelViewProj, textureId);
}

void Chunk::render_foliage(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId) {
    mesh_foliage.render(modelViewProjMatrixLocation, modelViewProj, textureId);
}

void Chunk::render_transparent(int modelViewProjMatrixLocation, const GLfloat* modelViewProj, GLuint textureId) {
    mesh_transparent.render(modelViewProjMatrixLocation, modelViewProj, textureId);
}

void Chunk::calculateNumFaces() {
    num_faces = 0;
    num_faces_transparent = 0;
    num_faces_foliage = 0;
    for (int x = 0; x < CX; x++) {
        for (int y = 0; y < CY; y++) {
            for (int z = 0; z < CZ; z++) {
                int id = blocks[x][y][z].getId();
                if (id != 0) {
                    BlockType* type = BlockTypeManager::GetBlockType(id);
                    if (type->isTransparent()==Solid && type->hasCollision()){
                        if (getBlockTypeInt(x - 1, y, z) == 0) { // Todo: check for opeaqeness
                            num_faces++;
                            //std::cout << "x - 1 "<<  std::endl;
                        }
                        if (getBlockTypeInt(x + 1, y, z) == 0) { // Todo: check for opeaqeness
                            num_faces++;
                            //std::cout << "x + 1" << std::endl;
                        }
                        if (getBlockTypeInt(x, y - 1, z) == 0) { // Todo: check for opeaqeness
                            num_faces++;
                            //std::cout << "y - 1" << std::endl;
                        }
                        if (getBlockTypeInt(x, y + 1, z) == 0) { // Todo: check for opeaqeness
                            num_faces++;
                            //std::cout << "y + 1" << std::endl;
                        }
                        if (getBlockTypeInt(x, y, z - 1) == 0) { // Todo: check for opeaqeness
                            num_faces++;
                            //std::cout << "z - 1" << std::endl;
                        }
                        if (getBlockTypeInt(x, y, z + 1) == 0) { // Todo: check for opeaqeness
                            num_faces++;
                            //std::cout << "z + 1" << std::endl;
                        }
                    }
                    else if(!(type->isTransparent()==Solid))
                    {
                        num_faces_transparent += 6;
                    }
                    else{
                        num_faces += 6;
                    }
                }
            }
        }
    }
}

int Chunk::getNumMeshIndices() { // remove this fuction?
    return mesh.getNumIndices();
}
int Chunk::getMaxNumfaces() { // remove this fuction?
    return num_faces;
}
int Chunk::getNumfaces() { // remove this fuction?
    return mesh.getNumVertices()/4;
}

ChunkMesh* Chunk::getMesh() {
    return &mesh;
}

/* ChunkVertexBuffer* getVertexBuffer() {
    return &vertexBuffer;
}
IndexBuffer* getIndexBuffer() {
    return &indexBuffer;
}

void setVertexBuffer(ChunkVertexBuffer vb) {
    vertexBuffer = vb;
}
void setIndexBuffer(IndexBuffer ib) {
    indexBuffer = ib;
} */

glm::vec3 Chunk::getPos() {
    return pos;
}
void Chunk::setPos(glm::vec3 newPos) {
    pos = newPos;
    px = pos.x;
    py = pos.y;
    pz = pos.z;
}
unsigned int Chunk::get_numBlocks(){
    return numBlocks;
}
bool Chunk::is_empty(){
    return (numBlocks==0);
}

void Chunk::getIDPos(int*x, int*y, int*z){
    *x = px;
    *y = py;
    *z = pz;
}

static std::string getFormattedFilePath(const std::string& worldSavePath,  int pos_x, int pos_y, int pos_z){
    std::string file_path = worldSavePath + "/" + 
        std::to_string(pos_x) + "_" + std::to_string(pos_y) + "_" + std::to_string(pos_z)
        + ".bin";
    return file_path;
}

void Chunk::serialize(const std::string& worldSavePath){
    if (changed_since_loading){
        std::string file_path = getFormattedFilePath(worldSavePath, 
                                this->pos.x, this->pos.y, this->pos.z);
        FILE* fp = fopen(file_path.c_str(), "wb");
        if (fp != NULL){
            fwrite(&pos, sizeof(glm::vec3), 1, fp);
            fwrite(blocks, sizeof(Block) * CX * CY * CZ, 1, fp);
            fclose(fp);
        }
        else{
            std::cout << "couldn't write chunk: " << file_path << std::endl;
        }
    }
}

bool Chunk::deserialize(const std::string& worldSavePath, int load_x, int load_y, int load_z){
    std::string file_path = getFormattedFilePath(worldSavePath,
                            load_x, load_y, load_z);
    if(exists(file_path)){
        FILE* fp = fopen(file_path.c_str(), "rb");
        if (fp != NULL){
            std::cout << "loading chunk: " << file_path << std::endl;
            fread(&pos, sizeof(glm::vec3), 1, fp);
            fread(&blocks, sizeof(Block) * CX * CY * CZ, 1, fp);
            fclose(fp);
            changed = true;
            return true;
        }
        else{
            std::cout << "couldn't load chunk: " << file_path << std::endl;
        }
    }
    return false;
    
}

const unsigned long long Chunk::getId() {
    return id;
}
