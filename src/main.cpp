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
      sys.graphics().draw_text(1, 10, "UP");
   } 
   if (sys.input().is_button_held(BTN_DOWN)) {
      y++;
      sys.graphics().draw_text(1, 10, "DOWN");
   }
   if (sys.input().is_button_held(BTN_LEFT)) {
      x--;
      sys.graphics().draw_text(1, 10, "LEFT");
   }
   if (sys.input().is_button_held(BTN_RIGHT)) {
      x++;
      sys.graphics().draw_text(1, 10, "RIGHT");
   }

   sys.graphics().draw_text(1, 1, "HELLO WORLD");

   sys.graphics().draw_sprite(x, y, TEST_TEX);
}

int main(/*int argc, char* argv[]*/) {
   sys.set_update_function(update);
   sys.start();

   return 0;
}
