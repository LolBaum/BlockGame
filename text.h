#include <ft2build.h>
#include FT_FREETYPE_H

#include "glm/glm.hpp"
#include <glew.h>
#include <iostream>
#include <map>
#include <string>
#include "shader.h"
#include "SDL_handler.h"


#define FT_CONFIG_OPTION_ERROR_STRINGS 


struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class Font{
private:
    FT_Face face;
    
public:
    std::map<char, Character> Characters;
    int size; 
    Font(const char* name, FT_Library ft, int s = 30);
    ~Font();
    void load_character_glyph(unsigned char c);
};




class FontSystem
{
private:
    unsigned int VAO, VBO;
    Shader font_shader = Shader("shaders/font.vs", "shaders/font.fs");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(sdl_handler.getWidth()), 
                                      0.0f, static_cast<float>(sdl_handler.getHeight()));
public:
    FT_Library ft;
    FontSystem(){}
    int initialize(){
        if (FT_Init_FreeType(&ft)){
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return -1;
        }
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        return 0;
    }

    void RenderText(std::string text, Font font, float x, float y, float scale, glm::vec3 color)
    {
        // activate corresponding render state	
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        font_shader.bind();
        //glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
        glUniform3f(glGetUniformLocation(font_shader.getShaderId(), "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = font.Characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        font_shader.unbind();
        glDisable(GL_BLEND);
    }

    void render_multiline_text(std::string text, Font font, float x, float y, float scale, glm::vec3 color, float line_offset=5){
        std::string delimiter = "\n";
        std::string line;
        size_t last = 0; 
        size_t next = 0; 
        int i = 0;
        while ((next = text.find(delimiter, last)) != std::string::npos){
            line = text.substr(last, next-last);
            RenderText(line, font, x, y-(line_offset + font.size * scale)*i, scale, color);
            last = next + 1;
            i++;
        }
    }

    Font create_Font(const char* name, int size = 30){
        return Font(name, ft, size);
    }

    void load(){
        if (FT_Init_FreeType(&ft)){
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }
    }
    void unload(){
        FT_Done_FreeType(ft);
    }
    ~FontSystem(){}
};

//FontSystem Fontsys = FontSystem();


Font::Font(const char* name, FT_Library ft,int s){
    size = s;
    if (FT_New_Face(ft, name, 0, &face)){
        std::cout << "ERROR::FREETYPE: Failed to load font: " << name << std::endl;  
        // todo: add function for error logging and error handling
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++){
        //load_character_glyph(c);
        FT_Error error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (error != 0){
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                //puts(FT_Error_String(error), stderr);
            //std::cout << FT_Error_String(error) << std::endl;
            printf("%s\n", FT_Error_String(error));
            continue;
        }   
    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character character = {
        texture, 
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        (unsigned int)face->glyph->advance.x
    };
    Characters.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face);
}

void Font::load_character_glyph(unsigned char c){
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return;
    }
    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character character = {
        texture, 
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        (unsigned int)face->glyph->advance.x
    };
    Characters.insert(std::pair<char, Character>(c, character));
}
Font::~Font(){}








// String splitting from Stackoverflow https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

