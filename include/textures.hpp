#ifndef TEXTURES_H
#define TEXTURES_H

#pragma once
#include <glew.h>
#include <iostream>
#include <map>
#include <string>
#include "shader.hpp"
#include "definitions.hpp"


// forward declarations




class Texture
{
private:
	int32 textureWidth = 0;
	int32 textureHeight = 0;
	int32 bitsPerPixel = 0;
	GLuint textureId;
	
public:
	Texture(const char* filename, bool generate_mipmap=false);
    Texture();
    void load(const char* filename, bool generate_mipmap=false);


	GLuint get_textureId();
	GLuint* get_textureId_ptr();
	~Texture();
};

class StadardBlockTexture{
private:
	int x=0;
	int y=0;
public:
	StadardBlockTexture();
	StadardBlockTexture(int uv_x, int uv_y);

	virtual void get_coords(int* uv_x, int* uv_y);
	virtual void get_front(int* uv_x, int* uv_y);
	virtual void get_left(int* uv_x, int* uv_y);
	virtual void get_back(int* uv_x, int* uv_y);
	virtual void get_right(int* uv_x, int* uv_y);
	virtual void get_top(int* uv_x, int* uv_y);
	virtual void get_bottom(int* uv_x, int* uv_y);
    virtual void get_coords(int* uv_x, int* uv_y, BlockDirection side);

	~StadardBlockTexture();
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
	SpecialBlockTexture();
	SpecialBlockTexture(int front_x, int front_y, int left_x, int left_y, int back_x, int back_y, int right_x, int right_y,
						int top_x, int top_y, int bottom_x, int bottom_y);

	void get_coords(int* uv_x, int* uv_y);

	void get_front(int* uv_x, int* uv_y);
	void get_left(int* uv_x, int* uv_y);
	void get_back(int* uv_x, int* uv_y);
	void get_right(int* uv_x, int* uv_y);
	void get_top(int* uv_x, int* uv_y);
	void get_bottom(int* uv_x, int* uv_y);
    void get_coords(int* uv_x, int* uv_y, BlockDirection side);
	/* std::string get_info_string(){
		return std::string("Texture x: " << x << ", y: " << y)
	} */

	~SpecialBlockTexture();
};

#endif //TEXTURES_H