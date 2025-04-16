#include "sys/system.h"

#include <SDL2/SDL.h>

#include <cmath>
#include <array>

System sys;

typedef std::array<Color, 64> Sprite;

const Sprite TEST_TEX = {
   0, 0, 1, 1, 1, 1, 0, 0,
   0, 1, 1, 1, 1, 1, 1, 0,
   1, 1, 2, 1, 1, 2, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 2, 1, 1, 2, 1, 1,
   1, 1, 1, 2, 2, 1, 1, 1,
   0, 1, 1, 1, 1, 1, 1, 0,
   0, 0, 1, 1, 1, 1, 0, 0,
};

int x = SCREEN_WIDTH / 2;
int y = SCREEN_HEIGHT / 2;

void update() {

   if (sys.input().is_button_held(BTN_UP)) {
      y--;
   } 
   if (sys.input().is_button_held(BTN_DOWN)) {
      y++;
   }
   if (sys.input().is_button_held(BTN_LEFT)) {
      x--;
   }
   if (sys.input().is_button_held(BTN_RIGHT)) {
      x++;
   }

   sys.graphics().draw_sprite(x, y, TEST_TEX);
}

int main(/*int argc, char* argv[]*/) {
   sys.set_update_function(update);
   sys.start();

   return 0;
}
