#pragma once

#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "ast_visitor.h"
#include "ast_nodes.h"
#include "value.h"

#include <string>
#include <vector>

class Interpreter : public ASTVisitor {
   Lexer lexer;
   Parser parser;

   std::vector<Token> tokens;
public:
   Interpreter(std::string src)
      : lexer(src)
      {}

   std::vector<Token>& get_tokens();
   std::string token_to_str(Token t);

   Value* visit_literal_int(LiteralInt*);
   Value* visit_binary_op(BinaryOp*);
};
