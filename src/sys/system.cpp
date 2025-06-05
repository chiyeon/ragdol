#include "system.h"
#include "core/constants.h"

System::System()
{
   update = std::bind(&System::default_update, this);

   SDL_Init(SDL_INIT_VIDEO);

   SDL_CreateWindowAndRenderer(SCREEN_WIDTH * WINDOW_SCALE, SCREEN_HEIGHT * WINDOW_SCALE, 0, &window,
                               &renderer);

   frame_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                    SCREEN_WIDTH, SCREEN_HEIGHT);
}

void System::default_update() {}

void System::start()
{
   while (running)
   {
      while (SDL_PollEvent(&event))
      {
         if (event.type == SDL_QUIT)
            running = false;
      }

      input_.poll_input();

      graphics_.flush_vram();

      update();

      graphics_.update_frame_buffer(frame_buffer);
      draw_frame_buffer();

      SDL_Delay(16);
   }

   cleanup();
}

void System::draw_frame_buffer()
{
   SDL_RenderCopy(renderer, frame_buffer, nullptr, nullptr);
   SDL_RenderPresent(renderer);
}

void System::cleanup()
{
   SDL_DestroyTexture(frame_buffer);
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();
}
