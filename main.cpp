#include <SDL2/SDL.h>
#include <iostream>
#include <array>

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 128;
const int WINDOW_SCALE = 4;

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
         vram[index] = (x / 8 + y / 8) % 4;
      }
   }
}

int main(int argc, char* argv[]) {
   if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      std::cerr << "SDL init failed: " << SDL_GetError() << std::endl;
      return 1;
   }

   SDL_Window* window = SDL_CreateWindow(
         "fido",
         SDL_WINDOWPOS_CENTERED,
         SDL_WINDOWPOS_CENTERED,
         SCREEN_WIDTH * WINDOW_SCALE,
         SCREEN_HEIGHT * WINDOW_SCALE,
         SDL_WINDOW_SHOWN
      );

   SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
   SDL_Texture* texture = SDL_CreateTexture(
         renderer,
         SDL_PIXELFORMAT_RGBA8888,
         SDL_TEXTUREACCESS_STREAMING,
         SCREEN_WIDTH,
         SCREEN_HEIGHT
      );

   fill_varm_test();

   bool running = true;
   SDL_Event event;

   while (running) {
      while (SDL_PollEvent(&event)) {
         if (event.type == SDL_QUIT) running = false;
      }

      uint32_t* pixels;
      int pitch;
      SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch);
      for (int i = 0; i < vram.size(); i++) {
         SDL_Color c = PALETTE[vram[i] % 4];
         pixels[i] = (c.a << 24) | (c.r << 16) | (c.g << 8) | c.b;
      }
      SDL_UnlockTexture(texture);

      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, texture, nullptr, nullptr);
      SDL_RenderPresent(renderer);

      SDL_Delay(16);
   }

   SDL_DestroyTexture(texture);
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();

   return 0;
}
