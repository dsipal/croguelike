#include "Map.hpp"

Map::Map(int width, int height) : width(width), height(height) {
  floors.push_back(Floor(width, height));
}
Map::~Map() {}

void Map::addFloor() { floors.push_back(Floor(width, height)); }