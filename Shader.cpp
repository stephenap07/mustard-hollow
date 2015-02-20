#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>

#include "ScopeExit.hpp"
#include "Drawing.hpp"

using std::vector;

inline void PrintShaderError(GLuint shader, GLenum target, const char *filename)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint info_log_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_len);
        GLchar str_info_log[512];
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
}

inline void PrintShaderLinkError(GLuint program)
{
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint info_log_length;
        GLchar str_info_log[512];
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
        glGetProgramInfoLog(program, info_log_length, NULL, str_info_log);
        fprintf(stderr, "Linker failure: %s\n", str_info_log);
    }
}

/**
 * Compile the shader and return a GLuint
 */
const GLuint CreateShader(const char *filename, const GLenum target)
{
    std::ifstream fileStream(filename);
    if (!fileStream.is_open()) {
        std::cerr << "Woops, file " << filename << " can't be opened" << std::endl;
        exit(1);
    }
    std::string str((std::istreambuf_iterator<GLchar>(fileStream)),
            std::istreambuf_iterator<GLchar>());
    const GLchar *const_str = reinterpret_cast<const GLchar *>(str.c_str());
    GLuint shader = glCreateShader(target);
    glShaderSource(shader, 1, &const_str, NULL);
    glCompileShader(shader);
    PrintShaderError(shader, target, filename);

    return shader;
}

const GLuint CreateProgram(const vector<ShaderInfo> &shader_infos)
{
    GLuint program = glCreateProgram();
    for (auto info : shader_infos) {
        glAttachShader(program, info.shader);
    }
    glLinkProgram(program);
    PrintShaderLinkError(program);

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
