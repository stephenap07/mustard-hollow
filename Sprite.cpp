#include <vector>
#include "Sprite.hpp"


//==============================================================================
Sprite::Sprite()
{
    std::vector<ShaderInfo> shaders = {
        ShaderInfo("Triangles.vert", GL_VERTEX_SHADER),
        ShaderInfo("Triangles.frag", GL_FRAGMENT_SHADER)
    };    

    _program = CreateProgram(shaders);
    _vertexBuffer = CreateQuad(_program);
    _color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    _isTextured = 1;

    setSubRect(Rect({0.0f, 0.0f, 1.0f, 1.0f}));

    SetUniform(
        vertexBuffer().program,
        UniformType::k1i,
        "uni_is_textured",
        1,
        static_cast<GLvoid*>(&_isTextured)
    );

    SetUniform( 
        _program,
        UniformType::k4fv,
        "uni_color",
        1,
        glm::value_ptr(_color)
    );
}

//==============================================================================
Sprite::~Sprite()
{
    // TODO: Ownership of resources needs to be handled separately
    glDeleteTextures(1, &_textureInfo.texture);
    glDeleteProgram(_program);
    glDeleteBuffers(1, &_vertexBuffer.vbo);
    glDeleteBuffers(1, &_vertexBuffer.ebo);
}

//==============================================================================
const glm::mat4 Sprite::transform() const
{
    return glm::scale(
        glm::translate(
            glm::mat4(1.0f),
            glm::vec3(
                _position.x + _subRect.w/2.0f,
                _position.y + _subRect.h/2.0f, 0.0f
            )
        ),
        glm::vec3(_subRect.w/2.0f, -_subRect.h/2.0f, 1.0f)
    );
}

//==============================================================================
const glm::vec2 Sprite::position() const
{
    return _position;
}

//==============================================================================
void Sprite::setPosition(glm::vec2 position)
{
    _position = position;
}

//==============================================================================
void Sprite::setX(float x)
{
    _position.x = x;
}

//==============================================================================
void Sprite::setY(float y)
{
    _position.y = y;
}

//==============================================================================
const Rect Sprite::subRect() const
{
    return _subRect;
}

//==============================================================================
void Sprite::setSubRect(const Rect &subRect)
{
    _subRect = subRect;
    glm::vec4 textureXYUV = glm::vec4(
        _subRect.x / _textureInfo.w,
        _subRect.y / _textureInfo.h,
        _subRect.w / _textureInfo.w,
        _subRect.h / _textureInfo.h
        );
    SetUniform(_program, UniformType::k4fv, "uni_tex_xy_uv", 1,
               glm::value_ptr(textureXYUV));
}

//==============================================================================
const VertexBuffer Sprite::vertexBuffer() const
{
    return _vertexBuffer;
}

//==============================================================================
const TextureInfo Sprite::textureInfo() const
{
    return _textureInfo;
}

//==============================================================================
void Sprite::setTextureInfo(const TextureInfo &textureInfo)
{
    _textureInfo = textureInfo;
    setSubRect(Rect({0.0f, 0.0f, _textureInfo.w, _textureInfo.h}));
}
