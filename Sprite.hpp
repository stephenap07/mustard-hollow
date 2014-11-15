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
    glm::vec2 pos;
    Rect rect;
    VertexBuffer vbuffer;
    TextureInfo tinfo;
};

/* Creation Code */
Sprite CreateTexturedSprite(const char *texture_path, Rect rect);

/* Drawing Code */
void DrawSprite(const Sprite &sprite);

#endif
