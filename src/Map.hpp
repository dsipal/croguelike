#pragma once
#include "Floor.hpp"
#include <libtcod/bsp.hpp>
#include <libtcod/color.hpp>
#include <libtcod/console.hpp>
#include <libtcod/fov.hpp>
#include <vector>

class Map {
public:
  int width, height;
  int currentFloor = 0;
  std::vector<Floor> floors;
  Map(int width, int height);
  ~Map();
  Floor &activeFloor() { return floors[currentFloor]; }
  const Floor &activeFloor() const { return floors[currentFloor]; }
  void render(tcod::Console &console);

protected:
  friend class BspListener;
};