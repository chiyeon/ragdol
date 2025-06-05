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
   ReturnStatement* return_statement();
   Variable* variable();
   ASTNode* empty();

   /* parent statement */
   ASTNode* full_expr();

   /* list literal */
   ASTNode* list_expr();

   /* expressions */
   ASTNode* factor();
   ASTNode* term();
   ASTNode* expr();

   /* conditions */
   ASTNode* logical_or();
   ASTNode* logical_and();
   ASTNode* comparison();

   /* control flow */
   ASTNode* control_statement(); 

   /* loops */
   ASTNode* while_loop();

   FunctionDecl* function_decl();
   FunctionCall* function_call(bool returning = false);
};
