#include <libtcod/console.hpp>
#include <libtcod/sys.hpp>
#include <string_view>


int main()
{
  //window dimension and title
  constexpr int width = 80;
  constexpr int height = 50;
  constexpr std::string_view title = "libtcod C++ example";
  
  //init window
  int x = width / 2;
  int y = height / 2;
  TCODConsole::initRoot(width, height, title.data());

  //main loop  
  bool running = true;
  while (running) {
    // render
    TCODConsole::root->clear(); // Clear the console before we render anything
    TCODConsole::root->putchar(x, y, '@'); // Render an @ at the player position
    TCODConsole::root->flush(); // Flushing the console redraws it to the screen

    TCOD_key_t key {};
    static TCOD_event_t TCODSystem::waitForEvent(int eventMask, TCOD_key_t *key, TCOD_mouse_t *mouse, bool flush);

      switch (key.vk) {
    case TCODK_UP:
      y--;
      break;
    case TCODK_DOWN:
      y++;
      break;
    case TCODK_LEFT:
      x--;
      break;
    case TCODK_RIGHT:
      x++;
      break;
    case TCODK_ESCAPE:
      running = false;
      break;
    }
  }

  TCOD_quit();
  return 0;
}
