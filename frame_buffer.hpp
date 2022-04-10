#pragma once
#include <glew.h>

#include "definitions.hpp"
#include <iostream>
#include "SDL_handler.hpp"
#include "glm/glm.hpp"

// from: https://learnopengl.com/Advanced-OpenGL/Framebuffers

class FrameBuffer{
private:
    unsigned int FBO;
    unsigned int opaqueTexture;
    unsigned int depthTexture;
public:
    FrameBuffer(unsigned int opaqueTexture, unsigned int depthTexture){
        init(opaqueTexture, depthTexture);
    }
    FrameBuffer(){}

    void init(unsigned int opaqueTexture, unsigned int depthTexture){
        glGenFramebuffers(1, &FBO);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, opaqueTexture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: framebuffer is not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void init(unsigned int opaqueTexture){
        glGenFramebuffers(1, &FBO);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, opaqueTexture, 0);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: framebuffer is not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bind(){
        // glEnable(GL_DEPTH_TEST);
		// glDepthFunc(GL_LESS);
		// glDepthMask(GL_TRUE);
		// glDisable(GL_BLEND);

		// bind opaque framebuffer to render solid objects
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    unsigned int get_opaque_texture(){
        return opaqueTexture;
    }
    unsigned int get_depth_texture(){
        return depthTexture;
    }


};


class TransparentFrameBuffer{
private:
    unsigned int transparentFBO;
    unsigned int accumTexture;
    unsigned int revealTexture;
    unsigned int depthTexture;
    glm::vec4 zeroFillerVec = glm::vec4(0.0f);
	glm::vec4 oneFillerVec = glm::vec4(1.0f);
public:
    TransparentFrameBuffer(unsigned int accumTexture, unsigned int revealTexture, unsigned int depthTexture){
        init(accumTexture, revealTexture, depthTexture);
    }
    TransparentFrameBuffer(){}

    void init(unsigned int accumTexture, unsigned int revealTexture, unsigned int depthTexture){
        glGenFramebuffers(1, &transparentFBO);

        glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTexture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, revealTexture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

        const GLenum transparentDrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	    glDrawBuffers(2, transparentDrawBuffers);

        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: framebuffer is not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bind(){
        // configure render states
		// glDepthMask(GL_FALSE);
		// glEnable(GL_BLEND);
		// glBlendFunci(0, GL_ONE, GL_ONE);
		// glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
		// glBlendEquation(GL_FUNC_ADD);

		// // bind transparent framebuffer to render transparent objects
		glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
		// glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
		// glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);
    }
    unsigned int get_accum_texture(){
        return accumTexture;
    }
    unsigned int get_reveal_texture(){
        return revealTexture;
    }
    unsigned int get_depth_texture(){
        return depthTexture;
    }


};
