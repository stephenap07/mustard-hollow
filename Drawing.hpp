#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp> 

struct Sprite;

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

enum class UniformType {
    k1i = 0,
    k1f,
    k4fv,
    k2fv,
    kMatrix4fv,
    kMatrix3x4fv
};

enum class TextureType {
    kT2RL,
    kT2RN
};


/** Function Definitions **/

/* Shader Creation */
const GLuint CreateShader(const char *filename, const GLenum target);
const GLuint CreateProgram(const std::vector<ShaderInfo> &shader_infos);

/* Vertex initialization and drawing */
VertexBuffer CreateQuad(const GLuint program);
void DrawQuad(const VertexBuffer &buffer);

/* Uniform handling functions */
void SetUniform(GLuint program, UniformType type, const GLchar *name, GLsizei count, GLvoid *data);

/* Texture loading */
TextureInfo CreateTexture(const char *filename, TextureType tex_type);
GLuint CreateTextureFromSurface(SDL_Surface *surface, GLenum target);

/* Error Handling */
void HandleGLError(GLenum error);
