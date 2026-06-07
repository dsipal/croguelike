#include "Engine.hpp"
#include "Actor.hpp"
#include "Map.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_video.h>
#include <libtcod/color.hpp>
#include <libtcod/context.hpp>

Engine::Engine(int screen_width, int screen_height)
    : fovRadius(10), computeFov(true), console{screen_width, screen_height} {

  auto params = TCOD_ContextParams{};
  params.console = console.get();
  params.window_title = "libtcod C++ example";
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.vsync = true;
  context = tcod::Context(params);
  map = new Map(screen_width, screen_height);

  auto [start_x, start_y] = map->activeFloor().getPlayerStart();
  player = new Actor(start_x, start_y, '@', TCOD_ColorRGB{255, 255, 255});
  map->activeFloor().actors.push_back(player);
  map->activeFloor().computeFov(player->x, player->y, fovRadius);
  for (Room room : map->activeFloor().rooms) {
    int x = 0;
    while (x < room.MAX_MONSTERS) {
      map->activeFloor().populateRoom(room);
      x++;
    }
  }
}

void Engine::render() {
  map->activeFloor().render(console);
  for (Actor *actor : map->activeFloor().actors) {
    if (map->activeFloor().isInFov(actor->x, actor->y)) {
      actor->render(console);
    }
  }
}

Engine::~Engine() { delete map; }

std::vector<int> direction_user_should_move() {
  const bool *key_states = SDL_GetKeyboardState(NULL);
  int directionY = 0;
  int directionX = 0;

  if (key_states[SDL_SCANCODE_W]) {
    directionY += 1;
    engine.computeFov = true;
  }

  if (key_states[SDL_SCANCODE_S]) {
    directionY += -1;
    engine.computeFov = true;
  }

  if (key_states[SDL_SCANCODE_A]) {
    directionX += -1;
    engine.computeFov = true;
  }

  if (key_states[SDL_SCANCODE_D]) {
    directionX += 1;
    engine.computeFov = true;
  }

  return {directionY, directionX};
}

void Engine::update(SDL_Event *event) {
  context.convert_event_coordinates(*event);

  switch (event->type) {
  case SDL_EVENT_QUIT:
    exit(0);
  case SDL_EVENT_KEY_DOWN: {
    std::vector<int> dir = direction_user_should_move();
    int new_y = player->y - dir[0];
    int new_x = player->x + dir[1];

    if (console.in_bounds({new_x, new_y}) &&
        map->activeFloor().canWalk(new_x, new_y, player)) {
      player->y = new_y;
      player->x = new_x;
    }
    if (computeFov) {
      map->activeFloor().computeFov(player->x, player->y, fovRadius);
      computeFov = false;
    }
    break;
  }
  }
}
