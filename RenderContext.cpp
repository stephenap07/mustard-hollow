#include <SDL2/SDL_image.h>

#include "Drawing.hpp"
#include "Sprite.hpp"

static const char *DEFAULT_WINDOW_TITLE = "Mustard Hollow";
static const float DEFAULT_SCREEN_WIDTH = 360;
static const float DEFAULT_SCREEN_HEIGHT = 240;

RenderContext::RenderContext()
{
    init(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
}

RenderContext::RenderContext(const float screenWidth, const float screenHeight)
{
    init(screenWidth, screenHeight);
}

void RenderContext::init(const float screenWidth, const float screenHeight)
{
    /* SDL Initialization */
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF);

    m_window = SDL_CreateWindow(
        DEFAULT_WINDOW_TITLE, 100, 100,
        screenWidth, screenHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (m_window == nullptr) {
        fprintf(stderr, "Failed to create SDL_Window: %s\n", SDL_GetError());
    }

    m_openGLContext = SDL_GL_CreateContext(m_window);
    if (m_openGLContext == nullptr) {
        fprintf(stderr, "SDL_GL_CreateContext: Failed to created GL context\n");
    }

    glewExperimental = GL_TRUE;
    GLenum glew_error = glewInit();

    /* We can ignore this error https://www.opengl.org/wiki/OpenGL_Loading_Library */
    glGetError();

    if (glew_error != GLEW_OK) {
        fprintf(stderr, "glewInit failed, aborting\n");
    }

    m_world.setProjection(glm::ortho(0.0f, screenWidth, screenHeight, 0.0f));
    m_world.setView(glm::lookAt(
        glm::vec3(0, 0, 1), // pos
        glm::vec3(0, 0, 0), // look
        glm::vec3(0, 1, 0)  // up
    ));

}

RenderContext::~RenderContext()
{
    if (m_openGLContext) {
        SDL_GL_DeleteContext(m_openGLContext);
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    IMG_Quit();
    SDL_Quit();
}

SDL_Window *RenderContext::window()
{
    return m_window;
}

RenderWorld *RenderContext::world()
{
    return &m_world;
}

const float RenderContext::windowWidth() const
{
    return m_width;
}

const float RenderContext::windowHeight() const
{
    return m_height;
}

const std::string RenderContext::windowTitle() const
{
    return std::string(SDL_GetWindowTitle(m_window));
}

void RenderContext::setWindowTitle(const std::string &newTitle)
{
    SDL_SetWindowTitle(m_window, newTitle.c_str());
}

void RenderContext::swapWindow()
{
    SDL_GL_SwapWindow(m_window);
}

RenderWorld::RenderWorld()
{
    m_projection = glm::ortho(0.0f, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0.0f);
    m_view = glm::lookAt(
        glm::vec3(0, 0, 1), // pos
        glm::vec3(0, 0, 0), // look
        glm::vec3(0, 1, 0)  // up
    );
}

RenderWorld::RenderWorld(glm::mat4 newProjection, glm::mat4 newView) :m_projection(newProjection), m_view(newView)
{
}

const glm::mat4 RenderWorld::projection() const
{
    return m_projection;
}

void RenderWorld::setProjection(glm::mat4 newProjection)
{
    m_projection = newProjection;
}

const glm::mat4 RenderWorld::view() const
{
    return m_view;
}

void RenderWorld::setView(glm::mat4 newView)
{
    m_view = newView;
}

const glm::mat4 RenderWorld::transform() const
{
    return m_projection * m_view;
}

void RenderWorld::draw(Sprite *sprite) const
{
    glBindTexture(GL_TEXTURE_2D, sprite->textureInfo().texture);

    glm::mat4 model = transform() * sprite->transform();
    SetUniform(sprite->vertexBuffer().program, UniformType::kMatrix4fv, "uni_model", 1, glm::value_ptr(model));

    DrawQuad(sprite->vertexBuffer());
}
