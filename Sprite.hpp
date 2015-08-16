#ifndef H_SPRITE_H_
#define H_SPRITE_H_

#include "Drawing.hpp"

struct Rect {
    float x, y, w, h;
};

struct Sprite {
    Sprite (); 
    ~Sprite ();
    glm::mat4 getModel (const glm::mat4 &proj, const glm::mat4 &view) const;

    const glm::vec2 position () const;
    void setPosition (glm::vec2 position);
    void setX (float x);
    void setY (float y);
    const Rect subRect () const;
    void setSubRect (const Rect &subRect);
    const glm::mat4 transform () const;
    const VertexBuffer vertexBuffer () const;
    const TextureInfo textureInfo () const;
    void setTextureInfo (const TextureInfo &textureInfo);

private:
    glm::vec2 _position;
    Rect _subRect;
    VertexBuffer _vertexBuffer;
    TextureInfo _textureInfo;

    GLuint _program;
    GLuint _isTextured;
    glm::vec4 _color;
};

// Creation Code
Sprite CreateTexturedSprite (const char *texturePath, Rect rect);

#endif
