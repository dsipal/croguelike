#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <libtcod.hpp>

static tcod::Console g_console{80, 50};
static tcod::Context g_context;
static int player_x{40};
static int player_y{25};
static constexpr auto WHITE = tcod::ColorRGB{255, 255, 255};

SDL_AppResult SDL_AppInit(void **, int argc, char *argv[]) {
  auto params = TCOD_ContextParams{};
  params.console = g_console.get();
  params.window_title = "libtcod C++ example";
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.vsync = true;
  params.argc = argc;
  params.argv = argv;

  g_context = tcod::Context(params);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *) {
  g_console.clear();

  if (g_console.in_bounds({player_x, player_y})) {
    g_console.at({player_x, player_y}).ch = '@';
  }
  g_context.present(g_console);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *, SDL_Event *event) {
  g_context.convert_event_coordinates(*event);
  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *, SDL_AppResult) {
  TCOD_quit();
}
