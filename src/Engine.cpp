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

  auto [start_x, start_y] = map->floors[0].getPlayerStart();
  player = new Actor(start_x, start_y, '@', TCOD_ColorRGB{255, 255, 255});
  actors.push_back(player);
  map->floors[0].computeFov(player->x, player->y, fovRadius);
}

void Engine::render() {
  map->floors[0].render(console);
  for (Actor *actor : actors) {
    actor->render(console);
  }
}

Engine::~Engine() {
  for (Actor *actor : actors) {
    delete actor;
  }
  delete map;
}

std::vector<int> direction_user_should_move() {
  const bool *key_states = SDL_GetKeyboardState(NULL);
  int directionY = 0;
  int directionX = 0;

  /* (We're writing our code such that it sees both keys are pressed and cancels
   * each other out!) */
  if (key_states[SDL_SCANCODE_W]) {
    directionY += 1; /* pressed what would be "W" on a US QWERTY keyboard. Move
                        forward! */
    engine.computeFov = true;
  }

  if (key_states[SDL_SCANCODE_S]) {
    directionY += -1; /* pressed what would be "S" on a US QWERTY keyboard. Move
                         backward! */
    engine.computeFov = true;
  }

  if (key_states[SDL_SCANCODE_A]) {
    directionX +=
        -1; /* pressed what would be "A" on a US QWERTY keyboard. Move left! */
    engine.computeFov = true;
  }

  if (key_states[SDL_SCANCODE_D]) {
    directionX +=
        1; /* pressed what would be "D" on a US QWERTY keyboard. Move right! */
    engine.computeFov = true;
  }

  /* (In practice it's likely you'd be doing full directional input in here, but
   * for simplicity, we're just showing forward and backward) */

  return {directionY,
          directionX}; /* wasn't key in W or S location, don't move. */
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

    if (player->y >= map->height - 5) {
      map->addFloor();
    }

    if (player->x >= map->width - 5) {
      map->addFloor();
    }

    if (console.in_bounds({new_x, new_y}) &&
        !map->floors[0].isWall(new_x, new_y)) {
      player->y = new_y;
      player->x = new_x;
    }
    if (computeFov) {
      map->floors[0].computeFov(this->player->x, this->player->y, fovRadius);
      computeFov = false;
    }
    break;
  }
  }
}
