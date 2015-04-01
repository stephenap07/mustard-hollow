#include <cstdio>
#include <cstdlib>

#include "Drawing.hpp"

static const bool DEFAULT_IGNORE_UNUSED_UNIFORMS = true;

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

    GLushort vertIndices[] = { 0, 2, 3, 1 };

    buffer.program = program;

    glGenBuffers(1, &buffer.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertIndices), vertIndices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &buffer.vao);
    glBindVertexArray(buffer.vao);

    glGenBuffers(1, &buffer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_quad), vert_quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureVertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));

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

void SetUniform(GLuint program, UniformType type, const GLchar *name, GLsizei count, GLvoid *data)
{
    glUseProgram(program);

    /* Clear Errors */
    glGetError();
    GLint uniform = glGetUniformLocation(program, name);

    GLenum error = glGetError();

    if (error) {
        fprintf(stderr, "Error (%d) in shader program (%d), uniform (%s)\n", error, program, name);
    }

    if (uniform < 0) {
        if (!DEFAULT_IGNORE_UNUSED_UNIFORMS) {
            fprintf(stderr, "Invalid value for uniform (%s) in program (%d)\n", name, program);
        }
    } else if (uniform == GL_INVALID_VALUE) {
        fprintf(stderr, "Invalid program for uniform (%s) in program (%d)\n", name, program);
    } else if (uniform == GL_INVALID_OPERATION) {
        fprintf(stderr, "Invalid program operation for uniform (%s) in program (%d)\n", name, program);
    } else {
        switch (type) {
            case UniformType::k1i:
                glUniform1i(uniform, *static_cast<GLint*>(data));
                break;
            case UniformType::k1f:
                glUniform1f(uniform, *static_cast<GLfloat*>(data));
                break;
            case UniformType::k4fv:
                glUniform4fv(uniform, count, static_cast<GLfloat*>(data));
                break;
            case UniformType::k2fv:
                glUniform2fv(uniform, count, static_cast<GLfloat*>(data));
                break;
            case UniformType::kMatrix4fv:
                glUniformMatrix4fv(uniform, count, GL_FALSE, static_cast<GLfloat*>(data));
                break;
            case UniformType::kMatrix3x4fv:
                glUniformMatrix3x4fv(uniform, count, GL_FALSE, static_cast<GLfloat*>(data));
                break;
            default:
                fprintf(stderr, "Invalid uniform type\n");
                break;
        }
    }
}
