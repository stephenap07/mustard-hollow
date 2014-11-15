#include <SDL2/SDL_image.h>

#include "Drawing.hpp"


void Init(RenderContext *context, const float screen_width, const float screen_height)
{
    /* SDL Initialization */
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF);

    context->window = SDL_CreateWindow(
        WINDOW_TITLE, 100, 100,
        screen_width, screen_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (context->window == nullptr) {
        fprintf(stderr, "Failed to create SDL_Window: %s\n", SDL_GetError());
    }

    context->gl_context = SDL_GL_CreateContext(context->window);
    if (context->gl_context == nullptr) {
        fprintf(stderr, "1SDL_GL_CreateContext: FATAL - Failed to created GL context\n");
    }

    glewExperimental = GL_TRUE;
    GLenum glew_error = glewInit();

    /* We can ignore this error https://www.opengl.org/wiki/OpenGL_Loading_Library */
    glGetError();

    if (glew_error != GLEW_OK) {
        fprintf(stderr, "glewInit failed, aborting\n");
    }
}
