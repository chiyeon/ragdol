#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <array>

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 128;
const int WINDOW_SCALE = 5;

const std::array<SDL_Color, 4> PALETTE = {{
   { 0, 0, 0, 255 },
   { 255, 255, 255, 255 },
   { 255, 0, 0, 255 },
   { 0, 0, 255, 255 }
}};

std::array<uint8_t, SCREEN_WIDTH * SCREEN_HEIGHT> vram = {};

void fill_varm_test() {
   for (int y = 0; y < SCREEN_HEIGHT; ++y) {
      for (int x = 0; x < SCREEN_WIDTH; ++x) {
         int index = y * SCREEN_WIDTH + x;
         vram[index] = (int)(x * sin(x) * 2 + 2 + y) % 4;
      }
   }
}

void update_frame_buffer(SDL_Texture *frame_buffer) {
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

int main(int argc, char* argv[]) {
   if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
      return 1;
   }

   SDL_Event event;
   SDL_Renderer *renderer;
   SDL_Window *window;

   SDL_Texture *frame_buffer;
   bool running = true;

   SDL_CreateWindowAndRenderer(
         SCREEN_WIDTH * WINDOW_SCALE,
         SCREEN_HEIGHT * WINDOW_SCALE,
         0,
         &window, &renderer
      );
   frame_buffer = SDL_CreateTexture(
         renderer,
         SDL_PIXELFORMAT_RGBA8888,
         SDL_TEXTUREACCESS_STREAMING,
         SCREEN_WIDTH,
         SCREEN_HEIGHT
      );

   fill_varm_test();

   while (running) {
      while (SDL_PollEvent(&event)) {
         if (event.type == SDL_QUIT) running = false;
      }

      update_frame_buffer(frame_buffer);

      SDL_RenderClear(renderer);
      //SDL_Rect dest = { 0, 0, SCREEN_WIDTH * WINDOW_SCALE, SCREEN_HEIGHT * WINDOW_SCALE };
      SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
      SDL_RenderCopy(renderer, frame_buffer, nullptr, nullptr);
      SDL_RenderPresent(renderer);

      SDL_Delay(16);
   }

   SDL_DestroyTexture(frame_buffer);
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();

   return 0;
}
