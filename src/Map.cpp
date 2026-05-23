#include "libtcod.hpp"
#include "Map.hpp"
#include <libtcod/color.hpp>

Room generateRoom(int screen_width, int screen_height) {
    TCODRandom *rng = TCODRandom::getInstance();
    int roomX = rng->getInt(1, screen_width - 1);
    int roomY = rng->getInt(1, screen_height - 1);
    int roomWidth = rng->getInt(3, 10);
    int roomHeight = rng->getInt(3, 10);

    Room room = {roomX, roomY, roomWidth, roomHeight};
    return room;
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



void Map::drawRoom(Room room) {
    for (int x = room.x; x < room.x + room.width; x++) {
        for (int y = room.y; y < room.y + room.height; y++) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                Tile& tile = tiles[x + y * width];
                tile.ch = '.';
                tile.fg = {255, 255, 255};
            }
        }
    }
}

void Map::render(tcod::Console& console) {
    for (const Room& room : rooms) {
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