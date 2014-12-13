#ifndef TILE_MAP_H_
#define TILE_MAP_H_

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

#endif // TILE_MAP_H_
