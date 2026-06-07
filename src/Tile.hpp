#include <libtcod/color.h>
class Tile {
public:
  bool explored;
  int ch;
  TCOD_ColorRGB fg;
  Tile() : explored(false), ch(0), fg({255, 255, 255}) {}
};