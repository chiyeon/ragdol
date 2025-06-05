#pragma once

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "scope.h"
#include "sys/system.h"
#include "token.h"
#include "value.h"

#include <memory>
#include <stack>
#include <string>
#include <vector>

class ReturnException
{
 public:
   std::shared_ptr<Value> value;
   ReturnException(const std::shared_ptr<Value>& value) : value(value) {}
};

class Interpreter : public ASTVisitor<std::shared_ptr<Value>>
{
   /* parse text to ast */
   Lexer lexer;
   Parser parser;

   /* for game stuff */
   System system;
   /* passed into init_window as a function */
   ASTNode* update_body;

   void update() { update_body->accept(*this); }

   /* visitors */
   /*
   std::unique_ptr<ExprVisitor> exprVisitor;          // visit & interpret expressions
   std::unique_ptr<StatementVisitor> stmtVisitor;     // visit & run statements
                                                      // */

   /* tokens from lexer */
   std::vector<Token> tokens;
   /* pointer to root program of ast */
   ASTNode* ast;
   /* stack of values to return with function calls */
   std::stack<std::shared_ptr<Value>> return_buffer;

   /* pointer to global scope */
   std::shared_ptr<Scope> global_scope;
   /* current scope we are walking through */
   std::shared_ptr<Scope> current_scope;

 public:
   /* DEBUG PRINT HELPERS */
   static const int LOG_QUIET = 0;
   static const int LOG_NORMAL = 1;
   static const int LOG_VERBOSE = 2;
   int log_level = LOG_NORMAL;
   int log_scope = 0; /* tab in per scope */
   void log(std::string, int = LOG_NORMAL, char = '\n');

 private:
   void enter_new_scope();
   void exit_scope();

   /* updates var_name, throws if cannot find */
   void assign_or_insert_in_scope(const std::string&, std::shared_ptr<Value>);
   void assign_variable(const std::string& var_name, std::shared_ptr<Value> value);
   /* tries to update, if can't find creates new in current scope */
   void assign_or_insert_variable(const std::string& var_name, std::shared_ptr<Value> value);
   std::shared_ptr<Value> find_variable(const std::string& var_name);

 public:
   void print_variables();

   Interpreter(std::string src);
   ~Interpreter();

   std::vector<Token>& get_tokens();
   std::string token_to_str(Token t);
   ASTNode* parse();
   void interpret();

   /* EXPRESSION HANDLERS */
   std::shared_ptr<Value> visit_literal(Literal*) override;
   std::shared_ptr<Value> visit_binary_op(BinaryOp*) override;
   std::shared_ptr<Value> visit_unary_op(UnaryOp*) override;
   std::shared_ptr<Value> visit_variable(Variable*) override;
   std::shared_ptr<Value> visit_return(ReturnStatement*) override;

   /* STATEMENT HANDLERS */
   std::shared_ptr<Value> visit_block(Block*) override;
   std::shared_ptr<Value> visit_statement_list(StatementList*) override;
   std::shared_ptr<Value> visit_no_op(NoOp*) override;
   std::shared_ptr<Value> visit_assignment(Assignment*) override;
   std::shared_ptr<Value> visit_function_decl(FunctionDecl*) override;
   std::shared_ptr<Value> visit_function_call(FunctionCall*) override;
   std::shared_ptr<Value> visit_if_statement(IfStatement*) override;
   std::shared_ptr<Value> visit_while_loop(WhileLoop*) override;
};
