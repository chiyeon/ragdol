#include "sys/system.h"
#include "interpreter/interpreter.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <cmath>
#include <array>
#include <fstream>
#include <sstream>
#include <string>

// System sys;

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
int loglevel = 0;

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

std::string read_file(char* path) {
   std::ifstream file(path, std::ios::in | std::ios::binary);
   if (!file) {
      std::cerr << "Failed to open file: " << path << std::endl;
      return "";
   }

   std::ostringstream buffer;
   buffer << file.rdbuf();

   return buffer.str();
}

int main(int argc, char* argv[]) {
   if (argc < 2) {
      std::cerr << "USAGE: " << argv[0] << " path/to/file" << std::endl;
      return 1;
   }

   if (argc >= 3) {
      loglevel = atoi(argv[2]);
   }

   std::string INPUT = read_file(argv[1]);
   if (INPUT.length() == 0) return 1;

   if (loglevel > 0) {
      std::cout << "STARTING INTERPRETER WITH INPUT:" << std::endl;
      std::cout << "" << INPUT << std::endl;
   }

   Interpreter interpreter(INPUT);
   interpreter.log_level = loglevel;
   auto tokens = interpreter.get_tokens();

   /*
   std::cout << "Lexed tokens:" << std::endl;
   std::cout << "\t";
   for (auto t : tokens) {
      std::cout << t.line << ", " << t.column << "\t: " << t.to_str() << ", " << std::endl;
   }
   std::cout << std::endl;
   */

   ASTNode* ast = interpreter.parse();

   if (loglevel > 0) {
      std::cout << "Parsed AST:" << std::endl;
      std::cout << ast->to_str() << std::endl;

      std::cout << "Interpreting..." << std::endl;
   }

   interpreter.interpret();

   if (loglevel > 0) {
      interpreter.print_variables();
   }

   /*
   sys.set_update_function(update);
   sys.start();
   */

   return 0;
}
