#pragma once
#include <vector>
#include <libtcod/color.hpp>
#include <libtcod/console.hpp>

struct Tile {
    bool walkable;
    int ch;
    TCOD_ColorRGB fg;
    Tile() : walkable(true), ch(0), fg({255, 255, 255}) {}
};

struct Room {
    int x, y, width, height;
    Room(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
    int center_x() const { return x + width / 2; }
    int center_y() const { return y + height / 2; }
};

class Map {
    public:
        int width, height;
        Map(int width, int height);
        ~Map();
        bool isWall(int x, int y) const;
        void render(tcod::Console& console);


    protected:
        Tile *tiles;
        std::vector<Room> rooms;
        void setWall(int x, int y);
        void drawRoom(Room room);
};