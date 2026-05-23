#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <libtcod.hpp>
#include "Engine.hpp"
#include "Actor.hpp"

static int g_screen_width = 80;
static int g_screen_height = 50;

Engine engine(g_screen_width, g_screen_height);


SDL_AppResult SDL_AppInit(void **, int, char **) {

  for (int i = 0; i < 10; i++) {
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *) {
  engine.console.clear();

  engine.player->x = std::clamp(engine.player->x, 0, g_screen_width - 1);
  engine.player->y = std::clamp(engine.player->y, 0, g_screen_height - 1);

  engine.render();
  engine.context.present(engine.console);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *, SDL_Event *event) {
  engine.update(event);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *, SDL_AppResult) {
  TCOD_quit();
}
