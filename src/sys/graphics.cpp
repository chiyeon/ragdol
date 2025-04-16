#include "graphics.h"
#include "core/constants.h"

#include <SDL2/SDL.h>

#include <cmath>
#include <string>
#include <span>

void Graphics::update_frame_buffer(SDL_Texture *frame_buffer) {
   uint32_t *pixels;
   int pitch;
   
   SDL_LockTexture(frame_buffer, nullptr, (void**)&pixels, &pitch);
   SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
   for (int i = 0; i < vram.size(); ++i) {
      SDL_Color c = PALETTE[vram[i] % 4];
      pixels[i] = SDL_MapRGBA(format, c.r, c.g, c.b, c.a);
   }
   SDL_FreeFormat(format);
   SDL_UnlockTexture(frame_buffer);
}

void Graphics::flush_vram() {
   std::fill(vram.begin(), vram.end(), 0);
}

SDL_Color Graphics::get_color(Color c) {
   return PALETTE[c];
}

void Graphics::fill_vram_test() {
   for (int y = 0; y < SCREEN_HEIGHT; ++y) {
      for (int x = 0; x < SCREEN_WIDTH; ++x) {
         int index = y * SCREEN_WIDTH + x;
         vram[index] = (int)(x * sin(x) * 2 + 2 + y) % 4;
      }
   }
}

void Graphics::draw_sprite(int x, int y, std::span<const Color> s) {
   int sw = sqrt(s.size());
   for (int i = y; i < std::min(SCREEN_HEIGHT, y + sw); ++i) {
      for (int j = x; j < std::min(SCREEN_WIDTH, x + sw); ++j) {
         int vram_ind = i * SCREEN_WIDTH + j;
         int spr_ind = (i - y) * sw + (j - x);
         vram[vram_ind] = s[spr_ind];
      }
   }
}

void Graphics::draw_text(int x, int y, std::string s) {
   int offset = 0;

   for (int i = 0; i < s.size(); ++i) {
      if (isalpha(s[i])) {
         auto letter = fontface[s[i] - 'A'];
         draw_sprite(x + offset, y, letter);
      }

      offset += 6;
   }
}

void Graphics::draw_pixel(int x, int y, Color c) {
   vram[y * SCREEN_WIDTH + x] = c;
}

void Graphics::draw_line(int sx, int sy, int ex, int ey, Color c) {
   /*
    * Bresenham's line algorithm
    */

   int dx = std::abs(ex - sx);
   int dy = -std::abs(ey - sy);
   int sx_step = sx < ex ? 1 : -1;
   int sy_step = sy < ey ? 1 : -1;
   int err = dx + dy;

   while (true) {
      draw_pixel(sx, sy, c);

      if (sx == ex && sy == ey) break;

      int e2 = 2 * err;
      if (e2 >= dy) {
         err += dy;
         sx += sx_step;
      }
      if (e2 <= dx) {
         err += dx;
         sy += sy_step;
      }
   }

   int start_x = std::min(sx, ex);
   int end_x = std::max(ex, sx);

   int start_y = std::min(sy, ey);
   int end_y = std::max(ey, sy);

   for (int i = start_y; i < end_y; ++i) {
      for (int j = start_x; j < end_x; ++j) {
         if (i == j)
            draw_pixel(j, i, c);
      }
   }

}
