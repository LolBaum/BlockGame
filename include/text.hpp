

// 1) Comment like a smart person.
// 2) Use #define a lot. No, a LOT.
// 3) Don’t use variable names that will mock you.
// 4) Do error checking. You make errors. Yes, you.
// 5) Premature optimization is the root of all evil.

#pragma once
#include <string>


// Forward declarations
class Texture;
class TextMesh;


struct Font{
    Texture* font;
    TextMesh* mesh;
    int sheetSize;
    int glyphesPerRow;
    int glypheSizeX;
    int glypheSizeY;
    int emptySpace;
    int ASCII_offset;
    bool textHasChanged;
    float glypheRatio;
    //std::string textBuffer;
    bool renderShadow;
    float shadowBrightness;

    void loadFont(std::string fontPathName);

    Font();
    ~Font();

    void clear();

    void update();

    void _addLine(std::string text, float x, float y, float size, float brightness=1.0);
    void addLine(std::string text, float x, float y, float size, float brightness=1.0);
    void addMultipleLines(std::string text, float x, float y, float size, float line_offset=0.01);

    void render();



};


























// #include <ft2build.h>
// #include FT_FREETYPE_H

// #include "glm/glm.hpp"
// #include <glm/gtc/type_ptr.hpp>
// #include <glew.h>
// #include <iostream>
// #include <map>
// #include <string>
// #include <vector>
// #include "shader.hpp"
// #include "SDL_handler.hpp"

// 	// based on the turorial from learnopengl.com
// 	// https://learnopengl.com/In-Practice/Text-Rendering


// struct Character {
//     unsigned int TextureID;  // ID handle of the glyph texture
//     glm::ivec2   Size;       // Size of glyph
//     glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
//     unsigned int Advance;    // Offset to advance to next glyph
// };
// //std::map<char, Character> Characters;

// int load_face(FT_Face face, FT_Library ft, const char* pathname, std::map<char, Character>* chars, int font_size = 40){
// 	if (FT_New_Face(ft, pathname, 0, &face))
// 	{
// 		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
// 		return -1;
// 	}


// 	FT_Set_Pixel_Sizes(face, 0, font_size);
// 	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
// 	{
// 		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;  
// 		return -1;
// 	}

	
// 	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
// 	for (unsigned char c = 0; c < 128; c++)
// 	{
// 		// load character glyph 
// 		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
// 		{
// 			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
// 			continue;
// 		}
// 		// generate texture
// 		unsigned int texture;
// 		glGenTextures(1, &texture);
// 		glBindTexture(GL_TEXTURE_2D, texture);
// 		glTexImage2D(
// 			GL_TEXTURE_2D,
// 			0,
// 			GL_RED,
// 			face->glyph->bitmap.width,
// 			face->glyph->bitmap.rows,
// 			0,
// 			GL_RED,
// 			GL_UNSIGNED_BYTE,
// 			face->glyph->bitmap.buffer
// 		);
// 		// set texture options
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 		// now store character for later use
// 		Character character = {
// 			texture, 
// 			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
// 			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
// 			(unsigned int)face->glyph->advance.x
// 		};
// 		chars->insert(std::pair<char, Character>(c, character));
// 	}
// 	return 0;
// }

// class Font{
// private:  
// public:
//     //std::map<char, Character> Characters;
//     std::map<char, Character> Characters;
//     int size; 
//     FT_Face face;

//     Font(const char* name, FT_Library ft,int s){
//         size = s;
//         if (FT_New_Face(ft, name, 0, &face)){
//             std::cout << "ERROR::FREETYPE: Failed to load font: " << name << std::endl;  
//             // todo: add function for error logging and error handling
//         }
//         load_face(face, ft, name, &Characters, s);
//         FT_Done_Face(face);
//         //FT_Done_FreeType(ft);
//         /* glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
//         for (unsigned char c = 0; c < 128; c++){
//             //load_character_glyph(c);
//             if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
//                 std::cout << c << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//                 continue;
//             }
           
//         // generate texture
//         unsigned int texture;
//         glGenTextures(1, &texture);
//         glBindTexture(GL_TEXTURE_2D, texture);
//         glTexImage2D(
//             GL_TEXTURE_2D,
//             0,
//             GL_RED,
//             face->glyph->bitmap.width,
//             face->glyph->bitmap.rows,
//             0,
//             GL_RED,
//             GL_UNSIGNED_BYTE,
//             face->glyph->bitmap.buffer
//         );
//         // set texture options
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         // now store character for later use
//         Character character = {
//             texture, 
//             glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//             glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//             (unsigned int)face->glyph->advance.x
//         };
//         Characters.insert(std::pair<char, Character>(c, character));
//         }
//         FT_Done_Face(face); */
//     }

//     void load_character_glyph(unsigned char c){
//         if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
//             std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//             return;
//         }
//         // generate texture
//         unsigned int texture;
//         glGenTextures(1, &texture);
//         glBindTexture(GL_TEXTURE_2D, texture);
//         glTexImage2D(
//             GL_TEXTURE_2D,
//             0,
//             GL_RED,
//             face->glyph->bitmap.width,
//             face->glyph->bitmap.rows,
//             0,
//             GL_RED,
//             GL_UNSIGNED_BYTE,
//             face->glyph->bitmap.buffer
//         );
//         // set texture options
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//         // now store character for later use
//         Character character = {
//             texture, 
//             glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//             glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//             (unsigned int)face->glyph->advance.x
//         };
//         Characters.insert(std::pair<char, Character>(c, character));
//     }
//     ~Font(){}
// };




// class FontSystem
// {
// private:
    
    
//     glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SDL_handler::getWidth()), 
//                                       0.0f, static_cast<float>(SDL_handler::getHeight()));
//     bool loaded = false;

//     std::vector<float32> textMesh;
//     int usedCharacters = 0;
// public:
//     FT_Library ft;

//     unsigned int VAO, VBO;
//     Shader font_shader = Shader("shaders/font.vs", "shaders/font.fs");

//     FontSystem(){}
//     int initialize(){
//         if (FT_Init_FreeType(&ft)){
//             std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//             return -1;
//         }
//         glGenVertexArrays(1, &VAO);
//         glGenBuffers(1, &VBO);
//         glBindVertexArray(VAO);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         glBindVertexArray(0);
//         glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SDL_handler::getWidth()), 0.0f, static_cast<float>(SDL_handler::getHeight()));
//         font_shader.bind();
//         glUniformMatrix4fv(glGetUniformLocation(font_shader.getShaderId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//         return 0;
//     }



//     Font create_Font(const char* name, int size = 30){
//         return Font(name, ft, size);
//     }

//     void load(){
//         if (FT_Init_FreeType(&ft)){
//             std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//         }
//         else{
//             loaded = true;
//         }
//     }
//     void unload(){
//         if(loaded){
//             FT_Done_FreeType(ft);
//             loaded = false;
//         }
//     }
//     ~FontSystem(){
//         unload();
//     }

//     void reserveCapacity(int numberOfChars){
//         std::cout << "reserving mor capacity for text rendering" << std::endl;
//         textMesh.reserve(numberOfChars*24);
//     }

//     void clearTextMesh(){
//         usedCharacters = 0;
//         textMesh.clear();
//     }


//     void addLine(std::string text, Font font, float x, float y, float scale, glm::vec3 color){
//         if(textMesh.capacity() < (usedCharacters + text.length()) * 24){
//             reserveCapacity(usedCharacters + text.length() * 2);
//         }
//         std::map<char, Character> Characters = font.Characters;	
//         std::string::const_iterator c;
//         for (c = text.begin(); c != text.end(); c++)
//         {
//             Character ch = Characters[*c];

//             float xpos = x + ch.Bearing.x * scale;
//             float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

//             float w = ch.Size.x * scale;
//             float h = ch.Size.y * scale;

//             float vertices[24] = {
//                   xpos,     ypos + h,   0.0f, 0.0f ,            
//                   xpos,     ypos,       0.0f, 1.0f ,
//                   xpos + w, ypos,       1.0f, 1.0f ,

//                   xpos,     ypos + h,   0.0f, 0.0f ,
//                   xpos + w, ypos,       1.0f, 1.0f ,
//                   xpos + w, ypos + h,   1.0f, 0.0f           
//                 };

//             textMesh.insert(textMesh.end(), vertices, vertices + sizeof(vertices));

//             // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//             x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
//         }

//         usedCharacters += text.length();
//     }


//     void render(Font font, glm::vec3 color){
//                 // Disable depth testing -> overlaping Characters will be Displayed correctly
//         glDisable(GL_DEPTH_TEST);


//         glEnable(GL_BLEND);
//         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
//         font_shader.bind();
//         //glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
//         glUniform3f(glGetUniformLocation(font_shader.getShaderId(), "textColor"), color.x, color.y, color.z);
//         glActiveTexture(GL_TEXTURE0);
//         glBindVertexArray(VAO);


//         // render glyph texture over quad
//         glBindTexture(GL_TEXTURE_2D, font.Characters['H'].TextureID);
//         // update content of VBO memory
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         //glBufferSubData(GL_ARRAY_BUFFER, 0, usedCharacters*24, textMesh.data()); 
//         glBufferData(GL_ARRAY_BUFFER, sizeof(float32)*usedCharacters, textMesh.data(), GL_STATIC_DRAW);
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         // render quad
//         glDrawArrays(GL_TRIANGLES, 0, usedCharacters*24);

        
//         glBindVertexArray(0);
//         glBindTexture(GL_TEXTURE_2D, 0);
//         font_shader.unbind();
//         glDisable(GL_BLEND);
//         glEnable(GL_DEPTH_TEST);

//         //std::cout << "usedCharacters: " << usedCharacters << std::endl;
//     }






//     // Depricated Functions

//     void RenderText(std::string text, Font font, float x, float y, float scale, glm::vec3 color)
//     {   
//         // Disable depth testing -> overlaping Characters will be Displayed correctly
//         glDisable(GL_DEPTH_TEST);

//         // activate corresponding render state
//         std::map<char, Character> Characters = font.Characters;	
//         glEnable(GL_BLEND);
//         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
//         font_shader.bind();
//         //glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
//         glUniform3f(glGetUniformLocation(font_shader.getShaderId(), "textColor"), color.x, color.y, color.z);
//         glActiveTexture(GL_TEXTURE0);
//         glBindVertexArray(VAO);

//         // iterate through all characters
//         std::string::const_iterator c;
//         for (c = text.begin(); c != text.end(); c++)
//         {
//             Character ch = Characters[*c];

//             float xpos = x + ch.Bearing.x * scale;
//             float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

//             float w = ch.Size.x * scale;
//             float h = ch.Size.y * scale;
//             // update VBO for each character
//             float vertices[6][4] = {
//                 { xpos,     ypos + h,   0.0f, 0.0f },            
//                 { xpos,     ypos,       0.0f, 1.0f },
//                 { xpos + w, ypos,       1.0f, 1.0f },

//                 { xpos,     ypos + h,   0.0f, 0.0f },
//                 { xpos + w, ypos,       1.0f, 1.0f },
//                 { xpos + w, ypos + h,   1.0f, 0.0f }           
//             };
//             // render glyph texture over quad
//             glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//             // update content of VBO memory
//             glBindBuffer(GL_ARRAY_BUFFER, VBO);
//             glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
//             glBindBuffer(GL_ARRAY_BUFFER, 0);
//             // render quad
//             glDrawArrays(GL_TRIANGLES, 0, 6);
//             // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//             x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
//         }
//         glBindVertexArray(0);
//         glBindTexture(GL_TEXTURE_2D, 0);
//         font_shader.unbind();
//         glDisable(GL_BLEND);
//         glEnable(GL_DEPTH_TEST);
//     }

//     // String splitting from Stackoverflow https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

//     void render_multiline_text(std::string text, Font font, float x, float y, float scale, glm::vec3 color, float line_offset=5){
//         std::string delimiter = "\n";
//         std::string line;
//         size_t last = 0; 
//         size_t next = 0; 
//         int i = 0;
//         while ((next = text.find(delimiter, last)) != std::string::npos){
//             line = text.substr(last, next-last);
//             RenderText(line, font, x, y-(line_offset + font.size * scale)*i, scale, color);
//             last = next + 1;
//             i++;
//         }
//     }
// };

// //FontSystem Fontsys = FontSystem();

