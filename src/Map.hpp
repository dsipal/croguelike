#pragma once
#include <libtcod/bsp.hpp>
#include <libtcod/color.hpp>
#include <libtcod/console.hpp>
#include <utility>
#include <vector>

struct Tile {
  bool walkable;
  int ch;
  TCOD_ColorRGB fg;
  Tile() : walkable(false), ch(0), fg({255, 255, 255}) {}
};

struct Room {
  int x, y, width, height;
  Room(int x, int y, int width, int height)
      : x(x), y(y), width(width), height(height) {}
  int center_x() const { return x + width / 2; }
  int center_y() const { return y + height / 2; }
};

class Map {
public:
  int width, height, room_count;
  TCODBsp bsp;
  Map(int width, int height, int room_count);
  ~Map();
  bool isWall(int x, int y) const;
  void render(tcod::Console &console);
  std::pair<int, int> getPlayerStart() const;

protected:
  Tile *tiles;
  friend class BspListener;
  std::vector<Room> rooms;
  void setWall(int x, int y);
  void setFloor(int x, int y);
  void drawRoom(Room room);
  void drunkardsWalk();
};