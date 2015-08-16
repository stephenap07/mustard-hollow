#ifndef H_DRAWING_H_
#define H_DRAWING_H_

#include <vector>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp> 

struct Sprite;

class RenderWorld {
public:
    RenderWorld (glm::mat4 newProjection, glm::mat4 newView);
    RenderWorld ();

    const glm::mat4 projection () const;
    void setProjection (glm::mat4 newProjection);
    const glm::mat4 view () const;
    void setView (glm::mat4 newView);
    const glm::mat4 transform () const;
    void draw (Sprite *sprite) const;

private:
    glm::mat4 _projection;
    glm::mat4 _view;
};

class RenderContext {
public:
    RenderContext (float screen_width, float screen_height);
    RenderContext ();
    ~RenderContext ();

    SDL_Window *window ();
    RenderWorld *world ();
    const std::string windowTitle () const;
    void setWindowTitle (const std::string &newTitle);
    void swapWindow () const;
    void clearWindow () const;

    const float windowWidth () const;
    const float windowHeight () const;

private:
    void init (const float screenWidth, const float screenHeight);

    RenderWorld _world;
    SDL_Window *_window;
    SDL_GLContext _openGLContext;
    float _height;
    float _width;
};

struct ShaderInfo {
    ShaderInfo ();
    ShaderInfo (const char *filename, GLenum target);
    GLenum type;
    const char *filename;
    GLuint shader;
};

struct TextureInfo {
    float w = 0.0f, h = 0.0f;
    GLuint texture = 0;
};

struct VertexBuffer {
    GLuint program = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
};

enum class UniformType {
    k1i = 0,
    k1f,
    k4fv,
    k2fv,
    kMatrix4fv,
    kMatrix3x4fv
};

enum class TextureType {
    kT2RL = 0
};


/// Function Definitions

// Shader Creation
const GLuint CreateShader (const char *filename, const GLenum target);
const GLuint CreateProgram (const std::vector<ShaderInfo> &shaderInfos);

/// Vertex initialization and drawing
VertexBuffer CreateQuad (const GLuint program);
void DrawQuad (const VertexBuffer &buffer);

/// Uniform handling functions
void SetUniform (
    GLuint program,
    UniformType type,
    const GLchar *name,
    GLsizei count,
    GLvoid *data);

/// Texture loading
TextureInfo Create2DTexture (const char *filename, TextureType textureType);
GLuint Create2DTextureFromSurface (SDL_Surface *surface);

///  Error Handling
void HandleGLError (GLenum error);

#endif
