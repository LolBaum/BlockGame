#pragma once
#include <glew.h>

#include "definitions.hpp"
#include "frame_buffer.hpp"
#include "SDL_handler.hpp"
#include "shader.hpp"
#include "object_models.hpp"
//#include "util_funcs.hpp"
#include <iostream>

class Renderer{
private:
/*     float screenQuad[30] = {
		// positions		// uv
		-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,

		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};
    unsigned int quadVAO, quadVBO; */
    Quad screenQuad;
    unsigned int opaqueTexture;
    unsigned int depthTexture;

    unsigned int accumTexture;
    unsigned int revealTexture;

    FrameBuffer solidFB;
    TransparentFrameBuffer transparentFB;

    glm::vec4 zeroFillerVec = glm::vec4(0.0f);
	glm::vec4 oneFillerVec = glm::vec4(1.0f);

public:
    Renderer(){
        /* glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), screenQuad, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0); */
        screenQuad = Quad();

        unsigned int SCR_WIDTH = sdl_handler.getWidth();
        unsigned int SCR_HEIGHT = sdl_handler.getHeight();

        glGenTextures(1, &opaqueTexture);
        glBindTexture(GL_TEXTURE_2D, opaqueTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenTextures(1, &accumTexture);
        glBindTexture(GL_TEXTURE_2D, accumTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenTextures(1, &revealTexture);
        glBindTexture(GL_TEXTURE_2D, revealTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        solidFB.init(opaqueTexture, depthTexture);
        transparentFB.init(accumTexture, revealTexture, depthTexture);
    }

    void drawScreen(){
        /* glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6); */
        //GLCALL(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        screenQuad.draw();
    }

    void setModeSolid(){

        /* glEnable(GL_MULTISAMPLE);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
        
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);  */
        
        glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glClearColor(0.6, 0.7, 1.0, 1.0); // set Color to zero again and use a cubemap background

		// bind opaque framebuffer to render solid objects
		solidFB.bind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    /* void setModeText(){
        glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		solidFB.bind();
    } */
    void setModeTransparent(){
        glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunci(0, GL_ONE, GL_ONE);
		glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
		glBlendEquation(GL_FUNC_ADD);

		transparentFB.bind();
        glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
		glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);
    }

    void setModeComposite(){
        glDepthFunc(GL_ALWAYS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		solidFB.bind();
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, accumTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, revealTexture);
    }

    void setModeScreen(){
        glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE); // enable depth writes so glClear won't ignore clearing the depth buffer
		glDisable(GL_BLEND);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.6, 0.7, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, opaqueTexture);

    }

    FrameBuffer* getfb(){
        return &solidFB;
    }
    TransparentFrameBuffer* gettfb(){
        return &transparentFB;
    }

    unsigned int getopaqueTexture(){
        return opaqueTexture;
    }
    unsigned int getdepthTexture(){
        return depthTexture;
    }
    unsigned int getaccumTexture(){
        return accumTexture;
    }
    unsigned int getrevealTexture(){
        return revealTexture;
    }

};

/* class Renderer{
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


}; */