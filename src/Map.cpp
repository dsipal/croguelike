#include "Map.hpp"
#include "libtcod.hpp"
#include <libtcod/bsp.hpp>
#include <libtcod/color.hpp>

Room generateRoom(int width, int height) {
  TCODRandom *rng = TCODRandom::getInstance();

  // function will run until it generates a room with no overlaps to previous
  // rooms
  Room room = {rng->getInt(1, width - 20), rng->getInt(1, height - 20),
               rng->getInt(3, 20), rng->getInt(3, 20)};

  return room;
}

void Map::drunkardsWalk() {
  TCODRandom *rng = TCODRandom::getInstance();
  for (int i = 0; i < (int)rooms.size() - 1; i++) {
    int x = rooms[i].center_x();
    int y = rooms[i].center_y();
    int tx = rooms[i + 1].center_x();
    int ty = rooms[i + 1].center_y();
    while (x != tx || y != ty) {
      setFloor(x, y);
      setFloor(x + 1, y + 1);
      if (x == tx) {
        y += (ty > y) ? 1 : -1;
      } else if (y == ty) {
        x += (tx > x) ? 1 : -1;
      } else if (rng->getInt(0, 1)) {
        x += (tx > x) ? 1 : -1;
      } else {
        y += (ty > y) ? 1 : -1;
      }
    }
    setFloor(x, y);
  }
}

class BspListener : public ITCODBspCallback {
private:
  Map &map;
  int room_count;
  int lastx, lasty;
  TCODRandom *rng = TCODRandom::getInstance();

public:
  BspListener(Map &map) : map(map), room_count(0) {}

  bool visitNode(TCODBsp *node, void *) override {
    if (node->isLeaf()) {
      Room room = generateRoom(node->w - rng->getInt(3, 10),
                               node->h - rng->getInt(3, 10));
      map.rooms.push_back(room);
      return true;
    } else
      return false;
  }
};

Map::Map(int width, int height, int room_count)
    : width(width), height(height), room_count(room_count) {
  tiles = new Tile[width * height];
  TCODBsp bsp(0, 0, width, height);
  bsp.splitRecursive(NULL, 16, 16, 16, 2.0f, 1.5f);
  BspListener listener(*this);

  // iterate through the bsp and run the listener function
  bsp.traverseInvertedLevelOrder(&listener, NULL);

  //drunkardsWalk();
}
Map::~Map() { delete[] tiles; }
std::pair<int, int> Map::getPlayerStart() const {
  return {rooms[0].center_x(), rooms[0].center_y()};
}

bool Map::isWall(int x, int y) const { return !tiles[x + y * width].walkable; }

void Map::setWall(int x, int y) { tiles[x + y * width].walkable = false; }

void Map::setFloor(int x, int y) { tiles[x + y * width].walkable = true; }

void Map::drawRoom(Room room) {
  for (int x = room.x; x < room.x + room.width; x++) {
    for (int y = room.y; y < room.y + room.height; y++) {
      if (x >= 0 && x < width && y >= 0 && y < height) {
        Tile &tile = tiles[x + y * width];
        tile.walkable = true;
        tile.fg = {255, 255, 255};
      }
    }
  }
}

void Map::render(tcod::Console &console) {
  for (const Room &room : rooms) {
    drawRoom(room);
  }
  static const TCOD_ColorRGB darkWall{0, 0, 100};
  static const TCOD_ColorRGB darkGround{50, 50, 150};
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (console.in_bounds({x, y})) {
        const Tile &tile = tiles[x + y * width];
        console.at({x, y}).bg = isWall(x, y) ? darkWall : darkGround;
        if (tile.ch) {
          console.at({x, y}).ch = tile.ch;
          console.at({x, y}).fg = tile.fg;
        }
      }
    }
  }
}