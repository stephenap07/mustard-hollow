#ifndef H_SPRITE_H_
#define H_SPRITE_H_

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp> 

#include "Drawing.hpp"

struct Rect {
    float x, y, w, h;
};

struct SpriteProperties {
    GLuint program;
    bool is_textured;
    glm::vec4 texture_xy_uv;
    glm::vec4 color;
};

struct Sprite {
    glm::vec2 pos;
    Rect sub_rect;
    VertexBuffer vbuffer;
    TextureInfo tinfo;

    glm::mat4 getModel(const glm::mat4 &proj, const glm::mat4 &view) const;
};

/* Creation Code */
Sprite CreateTexturedSprite(const char *texture_path, Rect rect);

/* Drawing Code */
void DrawSprite(const RenderWorld &world, const Sprite &sprite);

#endif
