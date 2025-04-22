#pragma once

#include "token.h"
#include "ast_nodes.h"

#include <vector>

class Parser {
   std::vector<Token> tokens;
   size_t current = 0;

   Token advance();
   Token peek();
   bool is_at_end();

   void eat(TokenType type);

   void error(TokenType type);

public:
   Parser()
      {}

   void set_tokens(std::vector<Token> t) {
      current = 0;
      tokens = t;
   }

   ASTNode* factor();
   ASTNode* term();
   ASTNode* expr();

   ASTNode* parse();

   LiteralInt* lower_literal_int();
};
