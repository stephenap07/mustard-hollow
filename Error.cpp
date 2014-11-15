#include <cstdio>
#include <cstdlib>

#include "Drawing.hpp"


void HandleGLError(GLenum error)
{
    switch (error) {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            fprintf(stderr, "GL_ERROR: Invalid enum\n");
            break;
        case GL_INVALID_VALUE:
            fprintf(stderr, "GL_ERROR: Invalid value\n");
            break;
        case GL_INVALID_OPERATION:
            fprintf(stderr, "GL_ERROR: Invalid operation\n");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            fprintf(stderr, "GL_ERROR: Invalid framebuffer operation\n");
            break;
        case GL_OUT_OF_MEMORY:
            fprintf(stderr, "GL_ERROR: Out of memory\n");
            break;
        case GL_STACK_UNDERFLOW:
            fprintf(stderr, "GL_ERROR: Stack underflow\n");
            break;
        case GL_STACK_OVERFLOW:
            fprintf(stderr, "GL_ERROR: Stack overflow\n");
            break;
        default:
            break;
    };
}
