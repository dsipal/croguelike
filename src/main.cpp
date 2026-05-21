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

/* returns 1 if moving forward with this keypress, -1 if moving backward, 0 if not moving. */
std::vector<int> direction_user_should_move()
{
    const bool *key_states = SDL_GetKeyboardState(NULL);
    int directionY = 0;
    int directionX = 0;

    /* (We're writing our code such that it sees both keys are pressed and cancels each other out!) */
    if (key_states[SDL_SCANCODE_W]) {
        directionY += 1;  /* pressed what would be "W" on a US QWERTY keyboard. Move forward! */
    } 

    if (key_states[SDL_SCANCODE_S]) {
        directionY += -1;  /* pressed what would be "S" on a US QWERTY keyboard. Move backward! */
    }

    if (key_states[SDL_SCANCODE_A]) {
        directionX += -1;  /* pressed what would be "A" on a US QWERTY keyboard. Move left! */
  
    }

    if (key_states[SDL_SCANCODE_D]) {
        directionX += 1;  /* pressed what would be "D" on a US QWERTY keyboard. Move right! */
    }

    /* (In practice it's likely you'd be doing full directional input in here, but for simplicity, we're just showing forward and backward) */

    return {directionY, directionX};  /* wasn't key in W or S location, don't move. */
}



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

    //handle user keypresses for vertical movement
    case SDL_EVENT_KEY_DOWN: {
      std::vector<int> dir = direction_user_should_move();
      int new_y = player_y - dir[0];
      int new_x = player_x + dir[1];
      if (g_console.in_bounds({player_x, player_y})) {
        player_y = new_y;
        player_x = new_x;
      }
      break;
    }
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *, SDL_AppResult) {
  TCOD_quit();
}
