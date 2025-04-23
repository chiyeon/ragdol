#pragma once

#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "ast_visitor.h"
#include "ast.h"
#include "value.h"

#include <string>
#include <vector>
#include <unordered_map>

class Interpreter : public ASTVisitor {
   Lexer lexer;
   Parser parser;

   std::vector<Token> tokens;
   ASTNode* ast;

   bool debug_log = true;

public:
   // temp
   std::unordered_map<std::string, Value*> variables;
   void print_variables();

   Interpreter(std::string src);

   std::vector<Token>& get_tokens();
   std::string token_to_str(Token t);
   ASTNode* parse();
   void interpret();

   Value* visit_literal_int(LiteralInt*);
   Value* visit_binary_op(BinaryOp*);
   Value* visit_unary_op(UnaryOp*);
   Value* visit_block(Block*);
   Value* visit_no_op(NoOp*);
   Value* visit_assignment(Assignment*);
   Value* visit_variable(Variable*);
};
