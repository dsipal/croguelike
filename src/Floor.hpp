#include "Engine.hpp"
#include <libtcod/bsp.hpp>
#include <libtcod/color.h>
#include <libtcod/console.hpp>
#include <libtcod/fov.hpp>
#include <memory>
#include <vector>
struct Tile {
  bool walkable;
  bool explored;

  int ch;
  TCOD_ColorRGB fg;
  Tile() : walkable(false), explored(false), ch(0), fg({255, 255, 255}) {}
};
class BSPListener;
struct Room {
  int x, y, width, height;
  Room(int width, int height, int x, int y)
      : x(x), y(y), width(width), height(height) {}
  int center_x() const { return x + width / 2; }
  int center_y() const { return y + height / 2; }
};
class Floor {
public:
  int width, height;
  std::unique_ptr<TCODMap> map;
  std::vector<Room> rooms;
  Engine *engine;
  Floor(int width, int height);
  ~Floor();
  Floor(Floor &&) = default;
  Floor &operator=(Floor &&) = default;
  std::pair<int, int> getPlayerStart() const;
  bool isWall(int x, int y) const;
  bool isInFov(int x, int y);
  bool isExplored(int x, int y) const;
  void computeFov(int playerX, int playerY, int fovRadius);
  std::pair<int, int> getNorthWall(int room_index);
  std::pair<int, int> getSouthWall(int room_index);
  std::pair<int, int> getEastWall(int room_index);
  std::pair<int, int> getWestWall(int room_index);
  void render(tcod::Console &console);

protected:
  std::vector<Tile> tiles;
  friend class BSPListener;
  void setWall(int x, int y);
  void setFloor(int x, int y);
  void drawRoom(Room room);
  void drunkardsWalk();
};