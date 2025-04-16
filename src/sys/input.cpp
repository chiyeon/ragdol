#include "input.h"
#include "core/constants.h"

#include <SDL2/SDL.h>

#include <cstring>

Input::Input() {
   // fill states w default
   std::fill(input_state.begin(), input_state.end(), false);
   std::fill(prev_input_state.begin(), prev_input_state.end(), false);
}

void Input::poll_input() {
   // first set prev. this lets us know the exact frame that an input is first pressed
   prev_input_state = input_state;

   const Uint8 *state = SDL_GetKeyboardState(nullptr);

   input_state[BTN_UP]        = state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W];
   input_state[BTN_DOWN]      = state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S];
   input_state[BTN_LEFT]      = state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A];
   input_state[BTN_RIGHT]     = state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D];
   input_state[BTN_A]         = state[SDL_SCANCODE_Z] || state[SDL_SCANCODE_J];
   input_state[BTN_B]         = state[SDL_SCANCODE_X] || state[SDL_SCANCODE_K];
   input_state[BTN_C]         = state[SDL_SCANCODE_C] || state[SDL_SCANCODE_L];
   input_state[BTN_MENU]      = state[SDL_SCANCODE_ESCAPE];
}

bool Input::is_button_pressed(InputState i) {
   return input_state[i] && !prev_input_state[i];
}

bool Input::is_button_held(InputState i) {
   return input_state[i];
}

bool Input::is_button_released(InputState i) {
   return !input_state[i] && prev_input_state[i];
}
