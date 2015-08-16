#include <iostream>
#include <fstream>
#include <vector>

#include "ScopeExit.hpp"
#include "Drawing.hpp"

using std::vector;

namespace internal {

//==============================================================================
inline void PrintShaderError(GLuint shader, GLenum target, const char *filename)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        GLchar strInfoLog[512];
        glGetShaderInfoLog(shader, infoLogLen, NULL, strInfoLog);

        const char *shaderTypeCStr = NULL;

        if (target == GL_VERTEX_SHADER) {
            shaderTypeCStr = "vertex";
        } else if (target == GL_GEOMETRY_SHADER) {
            shaderTypeCStr = "geometry";
        } else if (target == GL_FRAGMENT_SHADER) {
            shaderTypeCStr = "fragment";
        }

        std::cerr << "Compile failure in "
                  << shaderTypeCStr
                  << " file: " << filename
                  << ", error: " << strInfoLog << std::endl;
    }
}

//==============================================================================
inline void PrintShaderLinkError(GLuint program)
{
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        GLchar strInfoLog[512];
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        std::cerr << "Linker failure: " << strInfoLog << std::endl;
    }
}

} // end namespace internal

//==============================================================================
const GLuint CreateShader(const char *filename, const GLenum target)
{
    std::ifstream fileStream(filename);
    if (!fileStream.is_open()) {
        std::cerr << "File " << filename << " can't be opened" << std::endl;
        exit(1);
    }
    std::string str((std::istreambuf_iterator<GLchar>(fileStream)),
            std::istreambuf_iterator<GLchar>());
    const GLchar *const_str = reinterpret_cast<const GLchar *>(str.c_str());
    GLuint shader = glCreateShader(target);
    glShaderSource(shader, 1, &const_str, NULL);
    glCompileShader(shader);
    internal::PrintShaderError(shader, target, filename);

    return shader;
}

//==============================================================================
const GLuint CreateProgram(const vector<ShaderInfo> &shaderInfos)
{
    GLuint program = glCreateProgram();
    for (const auto &info : shaderInfos) {
        glAttachShader(program, info.shader);
    }
    glLinkProgram(program);
    internal::PrintShaderLinkError(program);

    return program;
}

//==============================================================================
ShaderInfo::ShaderInfo() :type(0), filename(nullptr), shader(0)
{
}

//==============================================================================
ShaderInfo::ShaderInfo(const char *filename, const GLenum target)
{
    shader = CreateShader(filename, target);
    this->type = target;
    this->filename = filename;
}
