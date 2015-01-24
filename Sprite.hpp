#ifndef H_SPRITE_H_
#define H_SPRITE_H_

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp> 

#include "Drawing.hpp"

struct Rect {
    float x, y, w, h;
};

struct Sprite {
    Sprite(); 
    ~Sprite();

    void loadShaders();
    const glm::vec2 position() const;
    void setPosition(glm::vec2 position);
    void setX(float x);
    void setY(float y);
    void setTextureInfo(const TextureInfo &textureInfo);
    void setSubRect(const Rect &subRect);
    void setScale(const glm::vec2 &vec);
    const Rect subRect() const;
    const glm::mat4 transform() const;
    const VertexBuffer vertexBuffer() const;
    const TextureInfo textureInfo() const;

private:
    glm::vec2 m_position;
    glm::vec2 m_scale;
    Rect m_subRect;
    VertexBuffer m_vertexBuffer;
    TextureInfo m_textureInfo;

    GLuint m_program;
    bool m_isTextured;
    glm::vec4 m_color;
};

/* Creation Code */
Sprite CreateTexturedSprite(const char *texture_path, Rect rect);

#endif
