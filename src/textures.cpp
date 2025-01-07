

#include "textures.hpp"
//#include "core_includes.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "util_funcs.hpp"




Texture::Texture(const char* filename, bool generate_mipmap){
    load(filename, generate_mipmap);
}
Texture::Texture(){}
void Texture::load(const char* filename, bool generate_mipmap){
    stbi_set_flip_vertically_on_load(true);
    auto textureBuffer = stbi_load(filename, &textureWidth, &textureHeight, &bitsPerPixel, 4);
    if (textureBuffer == NULL){
        std::cout << "Cannot load texture " << filename <<": " << stbi_failure_reason() << std::endl;
    }
        
    
    GLCALL(glGenTextures(1, &textureId));
    GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
    //GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    //GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    if (textureBuffer) {
        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));
        if (generate_mipmap){
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
        }
    }

    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    
    if (textureBuffer) {
        stbi_image_free(textureBuffer);
    }
}

GLuint Texture::get_textureId(){
    return textureId;
}
GLuint* Texture::get_textureId_ptr(){
    return &textureId;
}
Texture::~Texture(){}






StadardBlockTexture::StadardBlockTexture(){}
StadardBlockTexture::StadardBlockTexture(int uv_x, int uv_y){
    x = uv_x;
    y = uv_y;
}

void StadardBlockTexture::get_coords(int* uv_x, int* uv_y){
    *uv_x = x;
    *uv_y = y;
}
void StadardBlockTexture::get_front(int* uv_x, int* uv_y){}
void StadardBlockTexture::get_left(int* uv_x, int* uv_y){}
void StadardBlockTexture::get_back(int* uv_x, int* uv_y){}
void StadardBlockTexture::get_right(int* uv_x, int* uv_y){}
void StadardBlockTexture::get_top(int* uv_x, int* uv_y){}
void StadardBlockTexture::get_bottom(int* uv_x, int* uv_y){}
/* std::string get_info_string(){
    return std::string("Texture x: " << x << ", y: " << y)
} */

StadardBlockTexture::~StadardBlockTexture(){}

void StadardBlockTexture::get_coords(int *uv_x, int *uv_y, BlockDirection side) {}


SpecialBlockTexture::SpecialBlockTexture() : StadardBlockTexture(){}
SpecialBlockTexture::SpecialBlockTexture(int front_x, int front_y, int left_x, int left_y, int back_x, int back_y, int right_x, int right_y,
                    int top_x, int top_y, int bottom_x, int bottom_y) : StadardBlockTexture(front_x, front_y){
    front = {front_x, front_y};
    left = {left_x, left_y};
    back = {back_x, back_y};
    right = {right_x, right_y};
    top = {top_x, top_y};
    bottom = {bottom_x, bottom_y};
}

void SpecialBlockTexture::get_coords(int* uv_x, int* uv_y) {
    *uv_x = front.x;
    *uv_y = front.y;
}

void SpecialBlockTexture::get_front(int* uv_x, int* uv_y){
    *uv_x = front.x;
    *uv_y = front.y;
}
void SpecialBlockTexture::get_left(int* uv_x, int* uv_y){
    *uv_x = left.x;
    *uv_y = left.y;
}
void SpecialBlockTexture::get_back(int* uv_x, int* uv_y){
    *uv_x = back.x;
    *uv_y = back.y;
}
void SpecialBlockTexture::get_right(int* uv_x, int* uv_y){
    *uv_x = right.x;
    *uv_y = right.y;
}
void SpecialBlockTexture::get_top(int* uv_x, int* uv_y){
    *uv_x = top.x;
    *uv_y = top.y;
}
void SpecialBlockTexture::get_bottom(int* uv_x, int* uv_y){
    *uv_x = bottom.x;
    *uv_y = bottom.y;
}
/* std::string get_info_string(){
    return std::string("Texture x: " << x << ", y: " << y)
} */

SpecialBlockTexture::~SpecialBlockTexture(){}

void SpecialBlockTexture::get_coords(int *uv_x, int *uv_y, BlockDirection side) {
    std::cout << "SpecialBlockTexture::get_coords: BlockDirection:" << side << std::endl;
    switch (side) {
        case X_negative:
            SpecialBlockTexture::get_left(uv_x, uv_y);
            break;
        case X_positive:
            SpecialBlockTexture::get_right(uv_x, uv_y);
            break;
        case Y_negative:
            SpecialBlockTexture::get_bottom(uv_x, uv_y);
            break;
        case Y_positive:
            SpecialBlockTexture::get_top(uv_x, uv_y);
            break;
        case Z_negative:
            SpecialBlockTexture::get_back(uv_x, uv_y);
            break;
        case Z_positive:
            SpecialBlockTexture::get_front(uv_x, uv_y);
            break;
        default:
            SpecialBlockTexture::get_coords(uv_x, uv_y);
            std::cout << "SpecialBlockTexture::get_coords: BlockDirection side is not defined "
                      << "value: " << side << std::endl;
            break;
    }
}

