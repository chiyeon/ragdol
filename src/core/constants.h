#pragma once

#define DEBUG_LOG 1

inline constexpr int SCREEN_WIDTH = 128;
inline constexpr int SCREEN_HEIGHT = 128;
inline constexpr int WINDOW_SCALE = 4;

enum InputState
{
   BTN_UP = 0,
   BTN_DOWN,
   BTN_LEFT,
   BTN_RIGHT,
   BTN_A,
   BTN_B,
   BTN_C,
   BTN_MENU
};
