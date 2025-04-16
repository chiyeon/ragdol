#pragma once

#include "core/constants.h"

#include <array>

class Input {
public:
   Input();

   void poll_input();

   bool is_button_pressed(InputState i);
   bool is_button_held(InputState i);
   bool is_button_released(InputState i);
private:
   std::array<bool, 8> input_state;
   std::array<bool, 8> prev_input_state;
};
