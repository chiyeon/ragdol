#pragma once

#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "ast.h"
#include "value.h"

#include <string>
#include <vector>
#include <unordered_map>

class Interpreter : public ASTVisitor<Value*> {
   /* parse text to ast */
   Lexer lexer;
   Parser parser;

   /* visitors */
   /*
   std::unique_ptr<ExprVisitor> exprVisitor;          // visit & interpret expressions
   std::unique_ptr<StatementVisitor> stmtVisitor;     // visit & run statements
                                                      // */

   /* tokens from lexer */
   std::vector<Token> tokens;
   /* pointer to root program of ast */
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

   /* EXPRESSION HANDLERS */
   Value* visit_literal_int(LiteralInt*) override;
   Value* visit_binary_op(BinaryOp*) override;
   Value* visit_unary_op(UnaryOp*) override;
   Value* visit_variable(Variable*) override;

   /* STATEMENT HANDLERS */
   Value* visit_block(Block*) override;
   Value* visit_no_op(NoOp*) override;
   Value* visit_assignment(Assignment*) override;
};
