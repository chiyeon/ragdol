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

float x = 0.0;

int sx = 0;
int sy = 0;

void update() {
   x += 0.1;
   sys.graphics().draw_sprite(sin(x) * 10 + 20, sin(x * 2) * 5 + 20, TEST_TEX);

   sys.graphics().draw_line(50, 50 + 25 * sin(x), 75, 75 + -25 * sin(x), 2);
   
   sx++;
   sy++;

   sys.graphics().draw_pixel(sx, sin(sy) * 3 + 20, 3);
}

int main(/*int argc, char* argv[]*/) {
   sys.set_update_function(update);
   sys.start();

   return 0;
}
