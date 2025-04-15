#pragma once

#include "core/constants.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <cmath>
#include <array>

typedef uint8_t Color;
typedef std::array<Color, 64> Sprite;

class Graphics {
public:
   void update_frame_buffer(SDL_Texture *frame_buffer);
   void flush_vram();

   SDL_Color get_color(Color c);

   void fill_vram_test();

   void draw_sprite(int x, int y, Sprite s);

   void draw_pixel(int x, int y, Color c);
   void draw_line(int sx, int sy, int ex, int ey, Color c);

private:
   std::array<Color, SCREEN_WIDTH * SCREEN_HEIGHT> vram = {};
   const std::array<SDL_Color, 4> PALETTE = {{
      { 0, 0, 0, 255 },
      { 255, 255, 255, 255 },
      { 255, 0, 0, 255 },
      { 0, 0, 255, 255 }
   }};
};
