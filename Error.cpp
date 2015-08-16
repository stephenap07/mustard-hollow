#include <iostream>

#include <GL/glew.h>


void HandleGLError(GLenum error)
{
    switch (error) {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            std::cerr << "GL_ERROR: Invalid enum" << std::endl;
            break;
        case GL_INVALID_VALUE:
            std::cerr << "GL_ERROR: Invalid value" << std::endl;
            break;
        case GL_INVALID_OPERATION:
            std::cerr << "GL_ERROR: Invalid operation" << std::endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "GL_ERROR: Invalid framebuffer operation" << std::endl;
            break;
        case GL_OUT_OF_MEMORY:
            std::cerr << "GL_ERROR: Out of memory" << std::endl;
            break;
        case GL_STACK_UNDERFLOW:
            std::cerr << "GL_ERROR: Stack underflow" << std::endl;
            break;
        case GL_STACK_OVERFLOW:
            std::cerr << "GL_ERROR: Stack overflow" << std::endl;
            break;
        default:
            break;
    };
}
