#include "Room.hpp"
#include "Tile.hpp"
#include <libtcod/bsp.hpp>
#include <libtcod/color.h>
#include <libtcod/console.hpp>
#include <libtcod/fov.hpp>
#include <memory>
#include <vector>

class Actor;
class BSPListener;

class Floor {
public:
  int width, height;
  static const int MAX_ROOM_MONSTERS = 3;
  std::unique_ptr<TCODMap> map;
  std::vector<Room> rooms;
  std::vector<Actor *> actors;
  Floor(int width, int height);
  ~Floor();
  Floor(Floor &&) = default;
  Floor &operator=(Floor &&) = default;
  void addMonster(int x, int y);
  void populateRoom(Room room);
  std::pair<int, int> getPlayerStart() const;
  std::pair<int, int> placeExit();
  bool isWall(int x, int y) const;
  bool canWalk(int x, int y, const Actor *mover) const;
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
  void setExit(int x, int y);
  void drawRoom(Room room);
  void drunkardsWalk();
};