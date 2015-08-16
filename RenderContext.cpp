#include <SDL2/SDL_image.h>

#include "Drawing.hpp"
#include "Sprite.hpp"

static const char *DEFAULT_WINDOW_TITLE = "Mustard Hollow";
static const float DEFAULT_SCREEN_WIDTH = 360;
static const float DEFAULT_SCREEN_HEIGHT = 240;

//==============================================================================
RenderContext::RenderContext ()
{
    init(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
}

//==============================================================================
RenderContext::RenderContext (const float screenWidth, const float screenHeight)
{
    init(screenWidth, screenHeight);
}

//==============================================================================
void RenderContext::init (const float screenWidth, const float screenHeight)
{
    // SDL Initialization
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
    );
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF);

    _window = SDL_CreateWindow(
        DEFAULT_WINDOW_TITLE, 100, 100,
        screenWidth, screenHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (_window == nullptr) {
        fprintf(stderr, "Failed to create SDL_Window: %s\n", SDL_GetError());
    }

    _openGLContext = SDL_GL_CreateContext(_window);
    if (_openGLContext == nullptr) {
        fprintf(stderr, "SDL_GL_CreateContext: Failed to created GL context\n");
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    // We can ignore this error
    // https://www.opengl.org/wiki/OpenGL_Loading_Library
    glGetError();

    if (glewError != GLEW_OK) {
        fprintf(stderr, "glewInit failed, aborting\n");
    }

    _world.setProjection(glm::ortho(0.0f, screenWidth, screenHeight, 0.0f));
    _world.setView(glm::lookAt(
        glm::vec3(0, 0, 1), // pos
        glm::vec3(0, 0, 0), // look
        glm::vec3(0, 1, 0)  // up
    ));

    // Default GL properties
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//==============================================================================
RenderContext::~RenderContext ()
{
    SDL_GL_DeleteContext(_openGLContext);
    SDL_DestroyWindow(_window);
    IMG_Quit();
    SDL_Quit();
}

//==============================================================================
SDL_Window *RenderContext::window ()
{
    return _window;
}

//==============================================================================
RenderWorld *RenderContext::world ()
{
    return &_world;
}

//==============================================================================
const float RenderContext::windowWidth () const
{
    return _width;
}

//==============================================================================
const float RenderContext::windowHeight () const
{
    return _height;
}

//==============================================================================
const std::string RenderContext::windowTitle () const
{
    return std::string(SDL_GetWindowTitle(_window));
}

//==============================================================================
void RenderContext::setWindowTitle (const std::string &newTitle)
{
    SDL_SetWindowTitle(_window, newTitle.c_str());
}

//==============================================================================
void RenderContext::swapWindow () const
{
    SDL_GL_SwapWindow(_window);
}

//==============================================================================
void RenderContext::clearWindow () const
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//==============================================================================
RenderWorld::RenderWorld ()
{
    _projection = glm::ortho(
        0.0f,
        DEFAULT_SCREEN_WIDTH,
        DEFAULT_SCREEN_HEIGHT,
        0.0f
    );
    _view = glm::lookAt(
        glm::vec3(0, 0, 1), // pos
        glm::vec3(0, 0, 0), // look
        glm::vec3(0, 1, 0)  // up
    );
}

//==============================================================================
RenderWorld::RenderWorld (
    glm::mat4 newProjection,
    glm::mat4 newView)
    : _projection(newProjection)
    , _view(newView)
{
}

//==============================================================================
const glm::mat4 RenderWorld::projection () const
{
    return _projection;
}

//==============================================================================
void RenderWorld::setProjection (glm::mat4 newProjection)
{
    _projection = newProjection;
}

//==============================================================================
const glm::mat4 RenderWorld::view () const
{
    return _view;
}

//==============================================================================
void RenderWorld::setView (glm::mat4 newView)
{
    _view = newView;
}

//==============================================================================
const glm::mat4 RenderWorld::transform () const
{
    return _projection * _view;
}

//==============================================================================
void RenderWorld::draw (Sprite *sprite) const
{
    glBindTexture(GL_TEXTURE_2D, sprite->textureInfo().texture);
    glm::mat4 model = transform() * sprite->transform();
    SetUniform(
        sprite->vertexBuffer().program,
        UniformType::kMatrix4fv,
        "uni_model",
        1,
        glm::value_ptr(model)
    );
    DrawQuad(sprite->vertexBuffer());
}
