#include "libtcod.hpp"
#include "Actor.hpp"

Actor::Actor(int x, int y, int ch, TCOD_ColorRGB col) : x(x), y(y), ch(ch), col(col) {};

void Actor::render(tcod::Console& console) const {
    if (console.in_bounds({x, y})) {
        console.at({x, y}).ch = ch;
        console.at({x, y}).fg = col;
    }
}