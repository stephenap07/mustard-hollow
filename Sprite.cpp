#include <vector>
#include "Sprite.hpp"


Sprite::Sprite()
{
    std::vector<ShaderInfo> shaders = {
        ShaderInfo("Triangles.vert", GL_VERTEX_SHADER),
        ShaderInfo("Triangles.frag", GL_FRAGMENT_SHADER)
    };    

    m_program = CreateProgram(shaders);
    m_vertexBuffer = CreateQuad(m_program);

    m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    m_isTextured = true;
    SetUniform(vertexBuffer().program, UniformType::k1i, "uni_is_textured", 1, (GLvoid*)&m_isTextured);
    SetUniform(m_program, UniformType::k4fv, "uni_color", 1,
            glm::value_ptr(m_color));
    setSubRect(Rect({0.0f, 0.0f, 1.0f, 1.0f}));
}

Sprite::~Sprite()
{
    /* TODO: Ownership of resources needs to be handled separately*/
    glDeleteTextures(1, &m_textureInfo.texture);
    glDeleteProgram(m_program);
    glDeleteBuffers(1, &m_vertexBuffer.vbo);
    glDeleteBuffers(1, &m_vertexBuffer.ebo);
}

const glm::mat4 Sprite::transform() const
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(m_position.x + m_subRect.w/2.0f, m_position.y + m_subRect.h/2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));
    model = glm::scale(model, glm::vec3(m_scale.x, m_scale.y, 1.0f));

    return model;
}

const glm::vec2 Sprite::position() const
{
    return m_position;
}

void Sprite::setPosition(glm::vec2 position)
{
    m_position = position;
}

void Sprite::setX(float x)
{
    m_position.x = x;
}

void Sprite::setY(float y)
{
    m_position.y = y;
}

const Rect Sprite::subRect() const
{
    return m_subRect;
}

void Sprite::setSubRect(const Rect &subRect)
{
    m_subRect = subRect;
    glm::vec4 textureXYUV = glm::vec4(
            m_subRect.x / textureInfo().w,
            m_subRect.y / textureInfo().h,
            m_subRect.w / textureInfo().w,
            m_subRect.h / textureInfo().h
            );
    SetUniform(m_program, UniformType::k4fv, "uni_tex_xy_uv", 1,
            glm::value_ptr(textureXYUV));
}

const VertexBuffer Sprite::vertexBuffer() const
{
    return m_vertexBuffer;
}

const TextureInfo Sprite::textureInfo() const
{
    return m_textureInfo;
}

void Sprite::setTextureInfo(const TextureInfo &textureInfo)
{
    m_textureInfo = textureInfo;
    setSubRect(Rect({0.0f, 0.0f, m_textureInfo.w, m_textureInfo.h}));
}

void Sprite::setScale(const glm::vec2 &vec)
{
    m_scale = vec;
}
