#pragma once

#include "graphics.h"

#include <SDL2/SDL.h>

#include <functional>

class System {
public:
   System();

   void start();

   void set_update_function(std::function<void()> fn) { update = fn; }

   Graphics& graphics() { return gfx; }

private:
   std::function<void()> update;
   void default_update();

   SDL_Event event;
   SDL_Renderer *renderer;
   SDL_Window *window;

   Graphics gfx;

   SDL_Texture *frame_buffer;

   bool running = true;

   void cleanup();

   void draw_frame_buffer();
};
