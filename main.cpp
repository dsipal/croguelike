#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <libtcod.hpp>


int main(int argc, char* argv[])
{
  //initialize the console and context
  auto console = tcod::Console{80,50};
  auto params = TCOD_ContextParams{};
  params.console = console.get();
  params.window_title = "libtcod C++ example";
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.vsync = true;
  params.argc = argc;
  params.argv = argv;

  auto context = tcod::Context(params);

  //main loop  
  while (1) {
    console.clear();
    tcod::print(console, {40,25}, "Hello libtcod!", std::nullopt, std::nullopt);
    context.present(console);


    //keypress events + main loop
    SDL_Event event;
    SDL_WaitEvent(&event);
    while (SDL_PollEvent(&event)) {
      context.convert_event_coordinates(event);
       switch (event.type) {
        case SDL_EVENT_QUIT:
          return 0;
      }
    }
  }

  TCOD_quit();
  return 0;
}
