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


#include "ScopeExit.hpp"

using std::vector;

const char *WINDOW_TITLE = "Mustard Hollow v0.0.1";
const float SCREEN_WIDTH = 640;
const float SCREEN_HEIGHT = 640;

struct RenderContext {
    SDL_Window *window;
    SDL_GLContext gl_context;
};

struct ShaderInfo {
    GLenum type;
    const char *filename;
    GLuint shader;
};

struct Rect {
    float x, y, w, h;
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

struct Sprite {
    Rect rect;
    VertexBuffer vbuffer;
    TextureInfo tinfo;
};

enum class UniType {
    k1i = 0,
    k4fv,
    k2fv,
    kMatrix4fv,
    kMatrix3x4fv
};

enum class TextureType {
    kT2RL = 0
};

/* System initialization */
void Init(RenderContext *context, const float screen_width, const float screen_height);

/* Shader Creation */
const GLuint CreateShader(const char *filename, const GLenum target);
const GLuint CreateProgram(const vector<ShaderInfo> &shader_infos);

/* Vertex initialization and drawing */
VertexBuffer CreateQuad(const GLuint program);
void DrawQuad(const VertexBuffer &buffer);
void DrawSprite(const Sprite &sprite);

/* Uniform handling functions */
void SetUniform(GLuint program, UniType type, const GLchar *name, GLsizei count, GLvoid *data);

/* Texture loading */
TextureInfo CreateTexture(const char *filename, TextureType tex_type);
GLuint CreateTextureFromSurface(SDL_Surface *surface, GLenum target);

/* Error Handling */
void HandleGLError(GLenum error);

int main(int argc, char *argv[])
{
    RenderContext context;

    SCOPE_EXIT(SDL_GL_DeleteContext(context.gl_context););
    SCOPE_EXIT(SDL_DestroyWindow(context.window));
    SCOPE_EXIT(IMG_Quit());
    SCOPE_EXIT(SDL_Quit());

    Init(&context, SCREEN_WIDTH, SCREEN_HEIGHT);

    vector<ShaderInfo> shaders = {
        {GL_VERTEX_SHADER, "triangles.vert",
         CreateShader("triangles.vert", GL_VERTEX_SHADER)},
        {GL_FRAGMENT_SHADER, "triangles.frag",
         CreateShader("triangles.frag", GL_FRAGMENT_SHADER)}
    };    
    const GLuint program = CreateProgram(shaders);

    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    SetUniform(program, UniType::k4fv, "uni_color", 1, glm::value_ptr(color));

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(16.0f / 640.0f));
     model = glm::scale(model, glm::vec3(10));
    SetUniform(program, UniType::kMatrix4fv, "uni_model", 1, glm::value_ptr(model));

    GLint is_textured = 1;
    SetUniform(program, UniType::k1i, "uni_is_textured", 1, (GLvoid*)&is_textured);

    TextureInfo tex_info = CreateTexture("smw_ground.png", TextureType::kT2RL);
    SCOPE_EXIT(glDeleteTextures(1, &tex_info.texture));

    VertexBuffer buffer = CreateQuad(program);

    Sprite sprite;
    sprite.vbuffer = buffer;
    sprite.tinfo = tex_info;
    sprite.rect = { 137.0f, 99.0f, 16.0f, 16.0f };

    glm::vec4 texture_xy_uv = glm::vec4(sprite.rect.x / tex_info.w, sprite.rect.y / tex_info.h, sprite.rect.w / tex_info.w, sprite.rect.h / tex_info.h);
    SetUniform(program, UniType::k4fv, "uni_tex_xy_uv", 1, glm::value_ptr(texture_xy_uv));

    bool do_quit = false;
    while (!do_quit) {
        
        /* Poll for Events */
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    do_quit = true;
                    break;
                case SDL_KEYUP:
                    if (ev.key.keysym.sym == SDLK_ESCAPE) {
                        do_quit = true;
                    }
                    break;
            }
        }

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindTexture(GL_TEXTURE_2D, sprite.tinfo.texture);
        DrawQuad(sprite.vbuffer);

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

    if (context->window == nullptr) {
        fprintf(stderr, "Failed to create SDL_Window: %s\n", SDL_GetError());
    }

    context->gl_context = SDL_GL_CreateContext(context->window);
    if (context->gl_context == nullptr) {
        fprintf(stderr, "SDL_GL_CreateContext: FATAL - Failed to created GL context\n");
    }

    glewExperimental = GL_TRUE;
    GLenum glew_error = glewInit();

    /* We can ignore this error https://www.opengl.org/wiki/OpenGL_Loading_Library */
    glGetError();

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
    SCOPE_EXIT(std::fclose(file_ptr));

    GLchar *content = nullptr;
    SCOPE_EXIT(if (content) delete [] content);

    long length = 0;

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
        SCOPE_EXIT(if (str_info_log) delete [] str_info_log);

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
        SCOPE_EXIT(if (str_info_log) delete [] str_info_log);

        glGetProgramInfoLog(program, info_log_length, NULL, str_info_log);
        fprintf(stderr, "Linker failure: %s\n", str_info_log);
    }

    return program;
}

VertexBuffer CreateQuad(const GLuint program)
{
    VertexBuffer buffer;

    struct TextureVertex {
        GLfloat x, y, z, s, t;
    };

    TextureVertex vert_quad[] = {
        // Position          // Texture Coordinates
        {-1.0f,  1.0f, 0.0f, 0.0f, 0.0f},
        { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f},
        {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f},
        { 1.0f,  1.0f, 0.0f, 1.0f, 0.0f},
    };

    GLushort vert_indices[] = { 0, 2, 3, 1 };

    buffer.program = program;

    glGenBuffers(1, &buffer.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vert_indices), vert_indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &buffer.vao);
    glBindVertexArray(buffer.vao);

    glGenBuffers(1, &buffer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_quad), vert_quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), (GLvoid*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return buffer;
}

void DrawQuad(const VertexBuffer &buffer)
{
    glUseProgram(buffer.program);
    glBindVertexArray(buffer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);

    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void SetUniform(GLuint program, UniType type, const GLchar *name, GLsizei count, GLvoid *data)
{
    glUseProgram(program);

    /* Clear Errors */
    glGetError();
    GLint uniform = glGetUniformLocation(program, name);
    GLenum error = glGetError();

    if (error) {
        fprintf(stderr, "Error (%d) in shader program (%d), uniform (%s)\n", error, program, name);
    }

    if (uniform == -1) {
        fprintf(stderr, "Invalid uniform type (%d) uniform (%s) for shader id\n", program, name);
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

GLuint CreateTextureFromSurface(SDL_Surface *surface, GLenum target)
{
	assert(surface != nullptr);

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(target, texture_id);

	GLenum format;
    GLint internal_format = GL_RGB8;
	switch (surface->format->BytesPerPixel) {
        case 1:
            format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
            break;
		case 4:
			format = (surface->format->Rmask == 0x000000ff) ? GL_RGBA : GL_BGRA;
            internal_format = GL_RGBA8;
			break;
		case 3:
			format = (surface->format->Rmask == 0x000000ff) ? GL_RGB : GL_BGR;
			break;
		default:
            fprintf(stderr, "Invalid format for surface: %d\n", surface->format->BytesPerPixel);
			assert(false);
	}

    switch(target) {
        case GL_TEXTURE_1D:
            glTexImage1D(
                target,
                0, internal_format,
                surface->w, 0,
                format, GL_UNSIGNED_BYTE,
                surface->pixels);
            break;
        case GL_TEXTURE_2D:
            glTexImage2D(
                target,
                0, internal_format,
                surface->w, surface->h, 0,
                format, GL_UNSIGNED_BYTE,
                surface->pixels);
            break;
        default:
            fprintf(stderr, "Unsupported texture target\n");
    }

	glBindTexture(target, 0);

    return texture_id;
}

TextureInfo CreateTexture(const char *filename, TextureType tex_type)
{
    TextureInfo tex_info;

    SDL_Surface *optimized_surface = nullptr;
	SDL_Surface *surface = IMG_Load(filename);
    SCOPE_EXIT(SDL_FreeSurface(optimized_surface));

    if (surface != nullptr) {
        optimized_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_FreeSurface(surface);
    }

    Uint32 colorkey = SDL_MapRGB(optimized_surface->format, 27, 49, 65);

    optimized_surface->format->Amask = 0xFF000000;
    optimized_surface->format->Ashift = 24;

    if (SDL_SetColorKey(optimized_surface, SDL_TRUE, colorkey) != 0) {
        fprintf(stderr, "Failed to set surface color key\n");
    }

    tex_info.w = optimized_surface->w;
    tex_info.h = optimized_surface->h;
    
    if (tex_type == TextureType::kT2RL) {
        tex_info.texture = CreateTextureFromSurface(optimized_surface, GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, tex_info.texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    return tex_info;
}

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

void DrawSprite(const Sprite &sprite)
{
}
