#pragma once
#include <glew.h>
#include <iostream>
#include <map>
#include <string>
#include "shader.hpp"



class Texture
{
private:
	int32 textureWidth = 0;
	int32 textureHeight = 0;
	int32 bitsPerPixel = 0;
	GLuint textureId;
	
public:
	Texture(const char* filename){
		load(filename);
	}
    Texture(){}
    void load(const char* filename){
        stbi_set_flip_vertically_on_load(true);
		auto textureBuffer = stbi_load(filename, &textureWidth, &textureHeight, &bitsPerPixel, 4);
		
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
    }

	GLuint get_textureId(){
		return textureId;
	}
	GLuint* get_textureId_ptr(){
		return &textureId;
	}
	~Texture(){}
};