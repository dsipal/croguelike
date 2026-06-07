#include "Room.hpp"
#include <libtcod/mersenne.hpp>

Room Room::generate(int node_width, int node_height, int node_x, int node_y) {
  TCODRandom *rng = TCODRandom::getInstance();
  int room_w = rng->getInt(node_width / 2, node_width - 2);
  int room_h = rng->getInt(node_height / 2, node_height - 2);
  int room_x = node_x + rng->getInt(1, node_width - room_w - 1);
  int room_y = node_y + rng->getInt(1, node_height - room_h - 1);
  return Room(room_w, room_h, room_x, room_y, rng->getInt(0, 10));
}
