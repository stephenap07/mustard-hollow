#ifndef H_SPRITE_H_
#define H_SPRITE_H_

#include "Drawing.hpp"

struct Rect {
    float x, y, w, h;
};

struct Sprite {
    Rect rect;
    VertexBuffer vbuffer;
    TextureInfo tinfo;
};

/* Drawing Code */
void DrawSprite(const Sprite &sprite);

#endif
