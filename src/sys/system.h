#pragma once

#include "graphics.h"
#include "input.h"

#include <SDL2/SDL.h>

#include <functional>

class System
{
 public:
   System();

   void start();

   void set_update_function(std::function<void()> fn) { update = fn; }

   Graphics& graphics() { return graphics_; }
   Input& input() { return input_; }

 private:
   std::function<void()> update;
   void default_update();

   SDL_Event event;
   SDL_Renderer* renderer;
   SDL_Window* window;

   Graphics graphics_;
   Input input_;

   SDL_Texture* frame_buffer;

   bool running = true;

   void cleanup();

   void draw_frame_buffer();
};
