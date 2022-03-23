#pragma once
#include <glew.h>

#include "definitions.hpp"
#include <iostream>
#include "SDL_handler.hpp"

class FrameBuffer{
private:
    unsigned int opaqueFBO;
    unsigned int transparentFBO;
    unsigned int opaqueTexture, depthTexture;
    unsigned int accumTexture, revealTexture;
    unsigned int rbo;


/*     unsigned int quadVAO, quadVBO;

    float quadVertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    }; */

	

public:
    FrameBuffer(){
        glGenFramebuffers(1, &opaqueFBO);
	    glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO); 

        glGenTextures(1, &opaqueTexture);
        glBindTexture(GL_TEXTURE_2D, opaqueTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1440, 900, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, opaqueTexture, 0);  

        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, sdl_handler.getWidth(), sdl_handler.getHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, opaqueTexture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            std::cout << "ERROR::FRAMEBUFFER:: opaque Framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);  


        unsigned int accumTexture;
        glGenTextures(1, &accumTexture);
        glBindTexture(GL_TEXTURE_2D, accumTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, sdl_handler.getWidth(), sdl_handler.getHeight(), 0, GL_RGBA, GL_HALF_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        unsigned int revealTexture;
        glGenTextures(1, &revealTexture);
        glBindTexture(GL_TEXTURE_2D, revealTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, sdl_handler.getWidth(), sdl_handler.getHeight(), 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTexture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, revealTexture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

        const GLenum transparentDrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, transparentDrawBuffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Transparent framebuffer is not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    void bind(){
        glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
		//glEnable(GL_DEPTH_TEST);
    }

    void bind_transparent(){
        glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
		//glEnable(GL_DEPTH_TEST);
    }

    void unbind(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

        // glBindVertexArray(quadVAO);
        // glDisable(GL_DEPTH_TEST);
		// glBindTexture(GL_TEXTURE_2D, get_texture_id());
		// glDrawArrays(GL_TRIANGLES, 0, 6); 
    }

    

    unsigned int get_texture_id(){
        return opaqueTexture;
    }

    unsigned int get_texture_id1(){
        return depthTexture;
    }

};