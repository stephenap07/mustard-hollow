/**
 * Artificial Intelligence Final Project
 * Stephen Pridham
 *
 */

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp> 


#include "defer.hpp"

using std::vector;

const char *WINDOW_TITLE = "Mustard Hollow v0.0.1";
const float SCREEN_WIDTH = 1280;
const float SCREEN_HEIGHT = 720;

struct RenderContext {
    SDL_Window *window;
    SDL_GLContext gl_context;
};

struct ShaderInfo {
    GLenum type;
    const char *filename;
    GLuint shader;
};

struct VertexBuffer {
    GLuint program;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};

enum class UniType {
    k1i,
    k4fv,
    k2fv,
    kMatrix4fv,
    kMatrix3x4fv,
};


/* System initialization */
void Init(RenderContext *, const float screen_width, const float screen_height);

/* Shader Creation */
const GLuint CreateShader(const char *filename, const GLenum target);
const GLuint CreateProgram(const vector<ShaderInfo> &shader_infos);

/* Vertex initialization and drawing */
VertexBuffer CreateQuad(const GLuint program);
void DrawQuad(const VertexBuffer &buffer);

/* Uniform handling functions */
void SetUniform(GLuint program, UniType type, const char *name, GLsizei count, GLvoid *data);

int main(int argc, char *argv[])
{
    RenderContext context;
    /** TODO: Destructors v.s. defer */
    auto defr_context = defer([&context] {
        SDL_GL_DeleteContext(context.gl_context);
        SDL_DestroyWindow(context.window);
    });
    auto defr_sdl_clean = defer(SDL_Quit);
    auto defr_sdlimg_clean = defer(IMG_Quit);

    Init(&context, SCREEN_WIDTH, SCREEN_HEIGHT);

    vector<ShaderInfo> shaders = {
        {GL_VERTEX_SHADER, "triangles.vert",
         CreateShader("triangles.vert", GL_VERTEX_SHADER)},
        {GL_FRAGMENT_SHADER, "triangles.frag",
         CreateShader("triangles.frag", GL_FRAGMENT_SHADER)}
    };    
    const GLuint program = CreateProgram(shaders);

    glm::vec4 color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    SetUniform(program, UniType::kMatrix4fv, "uni_color", 1, glm::value_ptr(color));
    SetUniform(program, UniType::kMatrix4fv, "uni_model", 1, glm::value_ptr(model));

    VertexBuffer buffer = CreateQuad(program);

    bool do_quit = false;
    while (!do_quit) {
        
        /* Poll for Events */
        SDL_Event mh_event;
        while (SDL_PollEvent(&mh_event)) {
            switch (mh_event.type) {
                case SDL_QUIT:
                    do_quit = true;
                    break;
                case SDL_KEYUP:
                    if (mh_event.key.keysym.sym == SDLK_ESCAPE) {
                        do_quit = true;
                    }
                    break;
            }
        }

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        DrawQuad(buffer);

        glFlush();
        SDL_GL_SwapWindow(context.window);
    }

    return 0;
}

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

    context->gl_context = SDL_GL_CreateContext(context->window);
    if (context->gl_context == nullptr) {
        fprintf(stderr, "SDL_GL_CreateContext: FATAL - Failed to created GL context\n");
    }

    glewExperimental = GL_TRUE;
    GLenum glew_error = glewInit();

    if (glew_error != GLEW_OK) {
        fprintf(stderr, "glewInit failed, aborting\n");
    }
}


/**
 * Compile the shader and return a GLuint
 */
const GLuint CreateShader(const char *filename, const GLenum target)
{
    FILE *file_ptr = std::fopen(filename, "rb");
    GLchar *content = nullptr;
    long length = 0;

    auto defr1 = defer(std::fclose, file_ptr);
    auto defr2 = defer([&content]{ if (content) delete [] content; });

    if (!file_ptr) {
        std::perror("fopen");
    }

    std::fseek(file_ptr, 0L, SEEK_END);
    length = std::ftell(file_ptr);
    content = new GLchar[length + 1];
    std::fseek(file_ptr, 0L, SEEK_SET);

    if (!content) {
        std::fprintf(stderr, "failed to allocate memory\n");
    }

    std::fread(content, sizeof content, length, file_ptr);
    content[length] = 0;

    const char *content_ptr = content;
    GLuint shader = glCreateShader(target);
    glShaderSource(shader, 1, &content_ptr, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint info_log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_len);
        GLchar *str_info_log = new GLchar[info_log_len + 1];
        auto defr3 = defer([&str_info_log]{ if (str_info_log) delete [] str_info_log; });

        glGetShaderInfoLog(shader, info_log_len, NULL, str_info_log);

        const char *shader_type_cstr = NULL;

        if (target == GL_VERTEX_SHADER) {
            shader_type_cstr = "vertex";
        } else if (target == GL_GEOMETRY_SHADER) {
            shader_type_cstr = "geometry";
        } else if (target == GL_FRAGMENT_SHADER) {
            shader_type_cstr = "fragment";
        }

        std::fprintf(stderr, "Compile failure in %s shader %s:\n %s\n",
                     shader_type_cstr, filename, str_info_log);
    }

    return shader;
}

const GLuint CreateProgram(const vector<ShaderInfo> &shader_infos)
{
    GLuint program = glCreateProgram();
    for (auto info : shader_infos) {
        glAttachShader(program, info.shader);
    }
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint info_log_length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

        GLchar *str_info_log = new GLchar[info_log_length + 1];
        auto defr3 = defer([&str_info_log]{ if (str_info_log) delete [] str_info_log; });

        glGetProgramInfoLog(program, info_log_length, NULL, str_info_log);
        fprintf(stderr, "Linker failure: %s\n", str_info_log);
    }

    return program;
}

VertexBuffer CreateQuad(const GLuint program)
{
    VertexBuffer buffer;
    GLfloat vert_quad[] = {
        -1.0f,  1.0f, -1.0f, // Top Left
        1.0f, -1.0f, 1.0f,   // Bottom Right
        -1.0f, -1.0f, 1.0f,  // Bottom Left
        1.0f,  1.0f, -1.0f   // Top Right
    };

    GLushort vert_indices[] = {
        0, 2, 3, 1
    };

    buffer.program = program;

    glGenBuffers(1, &(buffer.ebo));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vert_indices), vert_indices,
            GL_STATIC_DRAW);

    glGenVertexArrays(1, &(buffer.vao));
    glBindVertexArray(buffer.vao);
    glGenBuffers(1, &(buffer.vbo));
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_quad), vert_quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return buffer;
}

void DrawQuad(const VertexBuffer &buffer)
{
    glUseProgram(buffer.program);
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, nullptr);
}

void SetUniform(GLuint program, UniType type, const char *name, GLsizei count, GLvoid *data)
{
    glUseProgram(program);

    GLint uniform = glGetUniformLocation(program, name);
    GLenum error = glGetError();

    if (error) {
        fprintf(stderr, "Error (%d) in shader program (%d), uniform (%s)\n", error, program, name);
    }

    if (uniform == -1) {
        fprintf(stderr, "Invalid uniform type (%d) for shader id\n", program);
    } else {
        switch (type) {
            case UniType::k1i:
                glUniform1i(uniform, *((GLint*)data));
                break;
            case UniType::k4fv:
                glUniform4fv(uniform, count, (GLfloat*)data);
                break;
            case UniType::k2fv:
                glUniform2fv(uniform, count, (GLfloat*)data);
                break;
            case UniType::kMatrix4fv:
                glUniformMatrix4fv(uniform, count, GL_FALSE, (GLfloat*)data);
                break;
            case UniType::kMatrix3x4fv:
                glUniformMatrix3x4fv(uniform, count, GL_FALSE, (GLfloat*)data);
                break;
            default:
                fprintf(stderr, "Invalid uniform type\n");
                break;
        }
    }
}
