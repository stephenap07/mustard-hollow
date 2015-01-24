#pragma once

#include "Rendering.hpp"
#include "Sprite.hpp"

class TileMap {
public:
    ~TileMap();
    void load();
    void draw(const RenderWorld *world);
private:
    Sprite m_sprite;
    std::vector<std::vector<int>> m_map;
};
