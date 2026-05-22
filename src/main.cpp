#include <vector>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <libtcod.hpp>
#include "Engine.hpp"
#include "Actor.hpp"
#include <array>

static int g_screen_width = 80;
static int g_screen_height = 50;

static constexpr auto WHITE = tcod::ColorRGB{255, 255, 255};

std::array<std::array<int, 4>, 10> g_rooms;
std::array<std::array<int, 80>, 40> g_map;

Engine engine(g_screen_width, g_screen_height);

std::array<int, 4> generateRoom() {
    TCODRandom *rng = TCODRandom::getInstance();
    int roomX = rng->getInt(1, g_screen_width - 1);
    int roomY = rng->getInt(1, g_screen_height - 1);
    int roomWidth = rng->getInt(3, 10);
    int roomHeight = rng->getInt(3, 10);

    std::array<int, 4> roomDimensions = {roomX, roomY, roomWidth, roomHeight};
    return roomDimensions;
}

void drawRoom(std::array<int, 4> room) {
    for (int x = room[0]; x < room[0] + room[2]; x++) {
        for (int y = room[1]; y < room[1] + room[3]; y++) {
            if (engine.console.in_bounds({x, y})) {
                engine.console.at({x, y}).ch = '.';
                engine.console.at({x, y}).fg = WHITE;
            }
        }
    }
}



SDL_AppResult SDL_AppInit(void **, int argc, char *argv[]) {
  auto params = TCOD_ContextParams{};
  params.console = engine.console.get();
  params.window_title = "libtcod C++ example";
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.vsync = true;
  params.argc = argc;
  params.argv = argv;

  engine.context = tcod::Context(params);
  for (int i = 0; i < 10; i++) {
    g_rooms[i] = generateRoom();
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *) {
  engine.console.clear();

  engine.player->x = std::clamp(engine.player->x, 0, g_screen_width - 1);
  engine.player->y = std::clamp(engine.player->y, 0, g_screen_height - 1);

  for (const auto& room : g_rooms) {
    drawRoom(room);
  }

  if (engine.console.in_bounds({engine.player->x, engine.player->y})) {
    engine.console.at({engine.player->x, engine.player->y}).ch = '@';
  }
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
