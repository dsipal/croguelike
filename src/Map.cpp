#include "libtcod.hpp"
#include "Map.hpp"

Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width * height];
    setWall(30,22);
    setWall(50,22);
}
Map::~Map() {
    delete [] tiles;
}
bool Map::isWall(int x, int y) const {
    return !tiles[x+y*width].walkable;
}

void Map::setWall(int x, int y) {
    tiles[x+y*width].walkable=false;
}

void Map::render(tcod::Console& console) const {
    static const TCOD_ColorRGB darkWall{0,0,100};
    static const TCOD_ColorRGB darkGround{50,50,150};
    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {
            if (console.in_bounds({x, y})) {
                console.at({x, y}).bg = isWall(x,y) ? darkWall : darkGround;
            }
        }
    }
}