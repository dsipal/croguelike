#pragma once
#include <array>
#include <vector>
#include <libtcod/color.hpp>
#include <libtcod/console.hpp>

struct Tile {
    bool walkable;
    int ch;
    TCOD_ColorRGB fg;
    Tile() : walkable(true), ch(0), fg({255, 255, 255}) {}
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
        std::vector<std::array<int, 4>> rooms;
        void setWall(int x, int y);
        void drawRoom(std::array<int, 4> room);
};