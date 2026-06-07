#include "Map.hpp"

Map::Map(int width, int height) : width(width), height(height) {
  int currentFloor = 0;
  floors.push_back(Floor(width, height));
}
Map::~Map() {}