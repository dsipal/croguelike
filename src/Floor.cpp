#include "Floor.hpp"
#include "Actor.hpp"
#include <iostream>
#include <libtcod/bsp.hpp>
#include <libtcod/console_types.hpp>
#include <libtcod/mersenne.hpp>

Room generateRoom(int node_width, int node_height, int node_x, int node_y) {
  TCODRandom *rng = TCODRandom::getInstance();
  int room_w = rng->getInt(node_width / 2, node_width - 2);
  int room_h = rng->getInt(node_height / 2, node_height - 2);
  int room_x = node_x + rng->getInt(1, node_width - room_w - 1);
  int room_y = node_y + rng->getInt(1, node_height - room_h - 1);
  return Room(room_w, room_h, room_x, room_y);
}

class BSPListener : public ITCODBspCallback {
private:
  Floor &floor;
  int room_count;
  int lastx, lasty;

public:
  BSPListener(Floor &floor) : floor(floor), room_count(4) {}

  bool visitNode(TCODBsp *node, void *) override {
    if (node->isLeaf()) {
      std::cout << node->x << ", " << node->y << std::endl;
      Room room = generateRoom(node->w, node->h, node->x, node->y);
      floor.rooms.push_back(room);
      return true;
    } else
      return false;
  }
};

Floor::Floor(int width, int height) {
  this->width = width;
  this->height = height;
  map = std::make_unique<TCODMap>(width, height);
  tiles.resize(width * height);
  TCODBsp bsp(0, 0, width, height);
  bsp.splitRecursive(NULL, 4, 16, 16, 1.5, 1.5);
  BSPListener listener(*this);
  bsp.traverseInvertedLevelOrder(&listener, NULL);
  for (const Room &room : rooms) {
    drawRoom(room);
  }
  drunkardsWalk();
};

Floor::~Floor() = default;

std::pair<int, int> Floor::getPlayerStart() const {
  return {rooms[0].center_x(), rooms[0].center_y()};
}

std::pair<int, int> Floor::getNorthWall(int room_index) {
  return {rooms[room_index].center_x() + (rooms[room_index].height / 2),
          rooms[room_index].center_y()};
};

std::pair<int, int> Floor::getSouthWall(int room_index) {
  return {rooms[room_index].center_x() - (rooms[room_index].height / 2),
          rooms[room_index].center_y()};
};

std::pair<int, int> Floor::getWestWall(int room_index) {
  return {rooms[room_index].center_x(),
          rooms[room_index].center_y() - (rooms[room_index].width / 2)};
};

std::pair<int, int> Floor::getEastWall(int room_index) {
  return {rooms[room_index].center_x(),
          rooms[room_index].center_y() + (rooms[room_index].width / 2)};
};

bool Floor::isWall(int x, int y) const { return !map->isWalkable(x, y); }
bool Floor::isExplored(int x, int y) const {
  return tiles[x + y * width].explored;
}
bool Floor::isInFov(int x, int y) {
  if (map->isInFov(x, y)) {
    tiles[x + y * width].explored = true;
    return true;
  }
  return false;
};

void Floor::computeFov(int playerX, int playerY, int fovRadius) {
  map->computeFov(playerX, playerY, fovRadius);
}

void Floor::setWall(int x, int y) {
  tiles[x + y * width].walkable = false;
  map->setProperties(x, y, false, false);
}

void Floor::setFloor(int x, int y) {
  tiles[x + y * width].walkable = true;
  map->setProperties(x, y, true, true);
}

void Floor::drawRoom(Room room) {
  for (int x = room.x; x < room.x + room.width; x++) {
    for (int y = room.y; y < room.y + room.height; y++) {
      if (x >= 0 && x < width && y >= 0 && y < height) {
        setFloor(x, y);
        tiles[x + y * width].fg = {255, 255, 255};
      }
    }
  }
}

void Floor::drunkardsWalk() {
  TCODRandom *rng = TCODRandom::getInstance();
  for (int i = 0; i < (int)rooms.size() - 1; i++) {
    int x = rooms[i].center_x();
    int y = rooms[i].center_y();
    int tx = rooms[i + 1].center_x();
    int ty = rooms[i + 1].center_y();
    while (x != tx || y != ty) {
      setFloor(x, y);
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

void Floor::render(tcod::Console &console) {
  static const TCOD_ColorRGB darkWall{0, 0, 100};
  static const TCOD_ColorRGB darkGround{50, 50, 150};
  static const TCOD_ColorRGB lightWall{130, 110, 50};
  static const TCOD_ColorRGB lightGround{200, 180, 50};
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (isInFov(x, y)) {
        console.at({x, y}).bg = isWall(x, y) ? lightWall : lightGround;
      } else if (isExplored(x, y)) {
        console.at({x, y}).bg = isWall(x, y) ? darkWall : darkGround;
      }
    }
  }
};