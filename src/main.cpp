#include "sys/system.h"
#include "interpreter/interpreter.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <cmath>
#include <array>

//System sys;

typedef std::array<Color, 64> Sprite;

const Sprite TEST_TEX = {
   0, 0, 1, 1, 1, 1, 0, 0,
   0, 1, 1, 1, 1, 1, 1, 0,
   1, 1, 2, 1, 1, 2, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 2, 1, 1, 2, 1, 1,
   1, 1, 1, 2, 2, 1, 1, 1,
   0, 1, 1, 1, 1, 1, 1, 0,
   0, 0, 1, 1, 1, 1, 0, 0,
};

int x = SCREEN_WIDTH / 2;
int y = SCREEN_HEIGHT / 2;

/*
void update() {

   if (sys.input().is_button_held(BTN_UP)) {
      y--;
      sys.graphics().draw_text(1, 10, "UP");
   } 
   if (sys.input().is_button_held(BTN_DOWN)) {
      y++;
      sys.graphics().draw_text(1, 10, "DOWN");
   }
   if (sys.input().is_button_held(BTN_LEFT)) {
      x--;
      sys.graphics().draw_text(1, 10, "LEFT");
   }
   if (sys.input().is_button_held(BTN_RIGHT)) {
      x++;
      sys.graphics().draw_text(1, 10, "RIGHT");
   }

   sys.graphics().draw_text(1, 1, "HELLO WORLD");

   sys.graphics().draw_sprite(x, y, TEST_TEX);
}
*/

int main(/*int argc, char* argv[]*/) {

   std::string INPUT = "{\n"
      "  let x = 2;\n"
      "  let y = 4;\n"
      "  let answer = 2 * (4 * x) + (1 - y) * x + (x / y);\n"
      "  {\n"
      "     let a = x + y + answer;\n"
      "  }\n"
      "}";

   std::cout << "STARTING INTERPRETER WITH INPUT:" << std::endl;
   std::cout << "" << INPUT << std::endl;

   Interpreter interpreter(INPUT);
   auto tokens = interpreter.get_tokens();

   /*
   std::cout << "Lexed tokens:" << std::endl;
   std::cout << "\t";
   for (auto t : tokens) {
      std::cout << t.line << ", " << t.column << "\t: " << t.to_str() << ", ";
   }
   std::cout << std::endl;
   */

   std::cout << "Parsed AST:" << std::endl;
   ASTNode* ast = interpreter.parse();
   std::cout << ast->to_str() << std::endl;

   std::cout << "Interpreting..." << std::endl;
   interpreter.interpret();
   interpreter.print_variables();

   //sys.set_update_function(update);
   //sys.start();

   return 0;
}
