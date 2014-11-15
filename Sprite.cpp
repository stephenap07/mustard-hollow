#include "Sprite.hpp"


void DrawSprite(const Sprite &sprite)
{
    glBindTexture(GL_TEXTURE_2D, sprite.tinfo.texture);
    DrawQuad(sprite.vbuffer);
}
