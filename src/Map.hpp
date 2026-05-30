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
  std::vector<Floor> floors;
  Map(int width, int height);
  ~Map();
  void render(tcod::Console &console);
  void addFloor();
  void render() const;

protected:
  friend class BspListener;
};