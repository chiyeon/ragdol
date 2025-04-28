#pragma once

#include "token.h"
#include "ast.h"

#include <vector>

class Parser {
   std::vector<Token> tokens;
   size_t current = 0;

   Token advance();
   Token peek();
   Token peekpeek();
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

   ASTNode* parse();

   ASTNode* program();
   ASTNode* block();
   StatementList* statement_list();
   ASTNode* statement();
   ASTNode* assignment_statement(bool new_var = true);
   Variable* variable();
   ASTNode* empty();

   ASTNode* factor();
   ASTNode* term();
   ASTNode* expr();



   FunctionDecl* function_decl();
   FunctionCall* function_call();

   LiteralInt* lower_literal_int();
};
