#include "Sprite.hpp"


void DrawSprite(const RenderWorld &world, const Sprite &sprite)
{
    glBindTexture(GL_TEXTURE_2D, sprite.tinfo.texture);
    sprite.getModel(world.proj, world.view);
    DrawQuad(sprite.vbuffer);
}

glm::mat4 Sprite::getModel(const glm::mat4 &proj, const glm::mat4 &view) const
{
    glm::mat4 model = glm::mat4(1.0f);
    model = (proj * view) * model;
    model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f));
    model = glm::scale(model, glm::vec3(sub_rect.w/2, sub_rect.h/2, 1.0f));
    SetUniform(vbuffer.program, UniType::kMatrix4fv, "uni_model", 1, glm::value_ptr(model));

    return model;
}
