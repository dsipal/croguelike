#include "libtcod.hpp"

struct Tile {
    bool walkable;
    Tile() : walkable(true) {}
};

class Map {
    public:
        int width, height;
        Map(int width, int height);
        ~Map();
        bool isWall(int x, int y) const;
        void render(tcod::Console& console) const;
    protected:
        Tile *tiles;
        void setWall(int x, int y);
};