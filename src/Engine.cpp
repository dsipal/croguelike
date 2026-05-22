#include "libtcod.hpp"
#include "Actor.hpp"
#include "Map.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_video.h>
#include <libtcod/color.hpp>
#include <libtcod/context.hpp>
#include "Engine.hpp"

Engine::Engine(int screen_width, int screen_height) : console{screen_width, screen_height} {

    player = new Actor(40,25,'@',TCOD_ColorRGB{255,255,255});
    std::vector<Actor *> actors{player};
    actors.push_back(new Actor(40,23,'@',TCOD_ColorRGB{255,255,255}));
    this->actors = actors;

    auto params = TCOD_ContextParams{};
    params.console = console.get();
    params.window_title = "libtcod C++ example";
    params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
    params.vsync = true;
    context = tcod::Context(params);
    map = new Map(80,45);

}

Engine::~Engine() {
    for (Actor* actor : actors) {
        delete actor;
    }
    delete map;
}

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

void Engine::update(SDL_Event *event) {
    context.convert_event_coordinates(*event);
    switch (event->type) {
        case SDL_EVENT_QUIT:
            exit(0);
        case SDL_EVENT_KEY_DOWN: {
            std::vector<int> dir = direction_user_should_move();
            int new_y = player->y - dir[0];
            int new_x = player->x + dir[1];
        if (console.in_bounds({player->x, player->y})) {
            player->y = new_y;
            player->x = new_x;
        }
      break;
    }
  }
}
