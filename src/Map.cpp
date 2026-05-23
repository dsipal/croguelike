#include "libtcod.hpp"
#include "Map.hpp"
#include <libtcod/color.hpp>

std::array<int, 4> generateRoom(int screen_width, int screen_height) {
    TCODRandom *rng = TCODRandom::getInstance();
    int roomX = rng->getInt(1, screen_width - 1);
    int roomY = rng->getInt(1, screen_height - 1);
    int roomWidth = rng->getInt(3, 10);
    int roomHeight = rng->getInt(3, 10);

    std::array<int, 4> roomDimensions = {roomX, roomY, roomWidth, roomHeight};
    return roomDimensions;
}

Map::Map(int width, int height) : width(width), height(height) {
    tiles = new Tile[width * height];
    setWall(30,22);
    setWall(50,22);
    for (int i = 0; i < 10; i++) {
        rooms.push_back(generateRoom(width, height));
    }
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



void Map::drawRoom(std::array<int, 4> room) {
    for (int x = room[0]; x < room[0] + room[2]; x++) {
        for (int y = room[1]; y < room[1] + room[3]; y++) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                Tile& tile = tiles[x + y * width];
                tile.ch = '.';
                tile.fg = {255, 255, 255};
            }
        }
    }
}

void Map::render(tcod::Console& console) {
    for (std::array<int, 4> room : rooms) {
        drawRoom(room);
    }
    static const TCOD_ColorRGB darkWall{0,0,100};
    static const TCOD_ColorRGB darkGround{50,50,150};
    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {
            if (console.in_bounds({x, y})) {
                const Tile& tile = tiles[x + y * width];
                console.at({x, y}).bg = isWall(x,y) ? darkWall : darkGround;
                if (tile.ch) {
                    console.at({x, y}).ch = tile.ch;
                    console.at({x, y}).fg = tile.fg;
                }
            }
        }
    }
}