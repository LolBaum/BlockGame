#pragma once
#include <glew.h>

#include "definitions.hpp"
#include "frame_buffer.hpp"
#include "shader.hpp"
//#include "util_funcs.hpp"
#include <iostream>

class Renderer{
private:
    FrameBuffer frame_buffer;
    Shader screenShader = Shader("shaders/frame.vs", "shaders/frame.fs");
    unsigned int quadVAO, quadVBO;
    // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    float quadVertices[24] = { 
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

public:
    Renderer(){
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        frame_buffer = *new FrameBuffer();
    }
    void render_to_frame_buffer(){
        frame_buffer.bind();
    }
    void render_to_frame_buffer_transparent(){
        frame_buffer.bind_transparent();
    }

    void render_to_screen(){
        screenShader.bind();
        //frame_buffer.unbind();
        glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, frame_buffer.get_texture_id());
		glDrawArrays(GL_TRIANGLES, 0, 6); 
        screenShader.unbind();
    }

    
    // temp funcs

    FrameBuffer* get_fb(){
        return &frame_buffer;
    }

    float* get_quadVertices(){
        return quadVertices;
    }
    unsigned int get_vao(){
        return quadVAO;
    }


};