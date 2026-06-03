#pragma once
#include <libtcod/console_types.hpp>
#include <libtcod/context.hpp>
#include <vector>
class Actor;
class Map;

class Engine {
public:
  int fovRadius;
  bool computeFov;
  tcod::Console console;
  tcod::Context context;
  std::vector<Actor *> actors;
  Actor *player;
  Map *map;
  Engine(int screen_width, int screen_height);
  ~Engine();
  void update(SDL_Event *event);
  void render();
  
};

extern Engine engine;