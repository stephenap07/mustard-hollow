#ifndef H_DRAWING_H_
#define H_DRAWING_H_

#include <vector>
#include <string>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp> 

struct Sprite;

class RenderWorld {
public:
    RenderWorld(glm::mat4 newProjection, glm::mat4 newView);
    RenderWorld();

    const glm::mat4 projection() const;
    void setProjection(glm::mat4 newProjection);
    const glm::mat4 view() const;
    void setView(glm::mat4 newView);
    const glm::mat4 transform() const;
    void draw(Sprite *sprite) const;

private:
    glm::mat4 m_projection;
    glm::mat4 m_view;
};

class RenderContext {
public:
    RenderContext(float screen_width, float screen_height);
    RenderContext();
    ~RenderContext();

    SDL_Window *window();
    RenderWorld *world();
    const std::string windowTitle() const;
    void setWindowTitle(const std::string &newTitle);
    void swapWindow();

    const float windowWidth() const;
    const float windowHeight() const;

private:
    void init(const float screenWidth, const float screenHeight);

    RenderWorld m_world;
    SDL_Window *m_window;
    SDL_GLContext m_openGLContext;
    float m_height;
    float m_width;
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


#endif
