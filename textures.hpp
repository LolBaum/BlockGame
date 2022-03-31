#pragma once
#include <glew.h>
#include <iostream>
#include <map>
#include <string>
#include "shader.hpp"
#include "definitions.hpp"



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
		if (textureBuffer == NULL){
			cout << "Cannot load texture " << filename <<": " << stbi_failure_reason() << endl;
		}
    		
		
		GLCALL(glGenTextures(1, &textureId));
		GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
		//GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
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

class StadardBlockTexture{
private:
	int x=0;
	int y=0;
public:
	StadardBlockTexture(){}
	StadardBlockTexture(int uv_x, int uv_y){
		x = uv_x;
		y = uv_y;
	}

	virtual void get_coords(int* uv_x, int* uv_y){
		*uv_x = x;
		*uv_y = y;
	}
	virtual void get_front(int* uv_x, int* uv_y){}
	virtual void get_left(int* uv_x, int* uv_y){}
	virtual void get_back(int* uv_x, int* uv_y){}
	virtual void get_right(int* uv_x, int* uv_y){}
	virtual void get_top(int* uv_x, int* uv_y){}
	virtual void get_bottom(int* uv_x, int* uv_y){}
	/* std::string get_info_string(){
		return std::string("Texture x: " << x << ", y: " << y)
	} */

	~StadardBlockTexture(){}
};


class SpecialBlockTexture : public StadardBlockTexture{
private:
	TexCoords front = {0, 0};
	TexCoords left = {0, 0};
	TexCoords back = {0, 0};
	TexCoords right = {0, 0};
	TexCoords top = {0, 0};
	TexCoords bottom = {0, 0};
public:
	SpecialBlockTexture() : StadardBlockTexture(){}
	SpecialBlockTexture(int front_x, int front_y, int left_x, int left_y, int back_x, int back_y, int right_x, int right_y,
						int top_x, int top_y, int bottom_x, int bottom_y) : StadardBlockTexture(front_x, front_y){
		front = {front_x, front_y};
		left = {left_x, left_y};
		back = {back_x, back_y};
		right = {right_x, right_y};
		top = {top_x, top_y};
		bottom = {bottom_x, bottom_y};
	}

	void get_coords(int* uv_x, int* uv_y) {
		*uv_x = front.x;
		*uv_y = front.y;
	}

	void get_front(int* uv_x, int* uv_y){
		*uv_x = front.x;
		*uv_y = front.y;
	}
	void get_left(int* uv_x, int* uv_y){
		*uv_x = left.x;
		*uv_y = left.y;
	}
	void get_back(int* uv_x, int* uv_y){
		*uv_x = back.x;
		*uv_y = back.y;
	}
	void get_right(int* uv_x, int* uv_y){
		*uv_x = right.x;
		*uv_y = right.y;
	}
	void get_top(int* uv_x, int* uv_y){
		*uv_x = top.x;
		*uv_y = top.y;
	}
	void get_bottom(int* uv_x, int* uv_y){
		*uv_x = bottom.x;
		*uv_y = bottom.y;
	}
	/* std::string get_info_string(){
		return std::string("Texture x: " << x << ", y: " << y)
	} */

	~SpecialBlockTexture(){}
};

