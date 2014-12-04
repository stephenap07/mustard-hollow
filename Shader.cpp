#include <vector>
#include <cstdio>
#include <cstdlib>

#include "ScopeExit.hpp"
#include "Drawing.hpp"

using std::vector;

/**
 * Compile the shader and return a GLuint
 */
const GLuint CreateShader(const char *filename, const GLenum target)
{
    FILE *file_ptr = std::fopen(filename, "rb");
    SCOPE_EXIT(std::fclose(file_ptr););

    GLchar *content = nullptr;
    SCOPE_EXIT(if (content) delete [] content;);

    long length = 0;

    if (!file_ptr) {
        std::fprintf(stderr, "Failed to open %s\n", filename);
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
        SCOPE_EXIT(if (str_info_log) delete [] str_info_log;);

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
        SCOPE_EXIT(if (str_info_log) delete [] str_info_log;);

        glGetProgramInfoLog(program, info_log_length, NULL, str_info_log);
        fprintf(stderr, "Linker failure: %s\n", str_info_log);
    }

    return program;
}

ShaderInfo::ShaderInfo() :type(0), filename(nullptr), shader(0)
{
}

ShaderInfo::ShaderInfo(const char *filename, const GLenum target)
{
    shader = CreateShader(filename, target);
    this->type = target;
    this->filename = filename;
}
