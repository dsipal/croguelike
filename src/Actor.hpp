#pragma once
#include <libtcod/color.hpp>
#include <libtcod/console.hpp>
#include <libtcod/context.hpp>
class Actor {
    public:
    int x,y;
    int ch;
    TCOD_ColorRGB col;

    Actor(int x, int y, int ch, TCOD_ColorRGB col);
    void render(tcod::Console& console) const;
};