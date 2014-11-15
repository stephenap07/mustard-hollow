#ifndef H_DRAWING_H_
#define H_DRAWING_H_

#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


static const char *WINDOW_TITLE = "Mustard Hollow v0.0.1";
static const float SCREEN_WIDTH = 640;
static const float SCREEN_HEIGHT = 640;
static const bool IGNORE_UNUSED_UNIFORMS = false;


struct RenderContext {
    SDL_Window *window;
    SDL_GLContext gl_context;
};

struct ShaderInfo {
    GLenum type;
    const char *filename;
    GLuint shader;
};

struct TextureInfo {
    float w, h;
    GLuint texture;
};

struct VertexBuffer {
    GLuint program;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};

enum class UniType {
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


/** Function Definitions **/

/* System initialization */
void Init(RenderContext *context, const float screen_width, const float screen_height);

/* Shader Creation */
const GLuint CreateShader(const char *filename, const GLenum target);
const GLuint CreateProgram(const std::vector<ShaderInfo> &shader_infos);

/* Vertex initialization and drawing */
VertexBuffer CreateQuad(const GLuint program);
void DrawQuad(const VertexBuffer &buffer);

/* Uniform handling functions */
void SetUniform(GLuint program, UniType type, const GLchar *name, GLsizei count, GLvoid *data);

/* Texture loading */
TextureInfo CreateTexture(const char *filename, TextureType tex_type);
GLuint CreateTextureFromSurface(SDL_Surface *surface, GLenum target);

/* Error Handling */
void HandleGLError(GLenum error);


#endif
