#ifndef H_DRAWING_H_
#define H_DRAWING_H_

#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp> 


static const char *WINDOW_TITLE = "Mustard Hollow v0.0.1";
static const float SCREEN_WIDTH = 1027;
static const float SCREEN_HEIGHT = 720;
static const bool IGNORE_UNUSED_UNIFORMS = true;


struct RenderContext {
    SDL_Window *window;
    SDL_GLContext gl_context;

    void Init(float screen_width, float screen_height);
    ~RenderContext();
};

struct RenderWorld {
    glm::mat4 proj;
    glm::mat4 view;
};

struct ShaderInfo {
    ShaderInfo();
    ShaderInfo(const char *filename, GLenum target);
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
