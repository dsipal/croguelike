#pragma once
#include <libtcod/console_types.hpp>
#include <libtcod/context.hpp>
class Actor;
class Map;

class Engine {
public:
  int fovRadius;
  bool computeFov;
  tcod::Console console;
  tcod::Context context;
  Actor *player;
  Map *map;
  Engine(int screen_width, int screen_height);
  ~Engine();
  void update(SDL_Event *event);
  void render();
  
};

extern Engine engine;