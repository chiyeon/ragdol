#include "interpreter.h"
#include "token.h"

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

void Interpreter::log(std::string message, int loglv, char endline) {
   if (loglv > log_level) return;

   for (int i = 0; i < log_scope; ++i) std::cout << "\t";
   std::cout << message << endline;
}

void Interpreter::enter_new_scope() {
   current_scope = std::make_shared<Scope>(current_scope);
}

void Interpreter::exit_scope() {
   if (!current_scope->is_global()) {
      current_scope = current_scope->get_parent(); 
   }
}

void Interpreter::assign_variable(const std::string& var_name, std::shared_ptr<Value> value) {
   /*
    * assigns variable backwards to closest scope, throwing error
    * if it cannot find it
    */
   if (!current_scope->assign(var_name, value)) {
      std::cout << "ERROR: Could not find variable " << var_name << std::endl;
   }
}

void Interpreter::assign_or_insert_variable(const std::string& var_name, std::shared_ptr<Value> value) {
   /*
    * assigns variable backwards to closest scope, if it cannot find it
    * it will add it to the current scope
    */
   current_scope->assign_or_insert(var_name, value);
}

std::shared_ptr<Value> Interpreter::find_variable(const std::string& var_name) {
   return current_scope->get(var_name);
}

Interpreter::Interpreter(std::string src)
   : lexer(src)
{
   global_scope = std::make_shared<Scope>();
   current_scope = global_scope;

   tokens = lexer.tokenize();

   parser.set_tokens(tokens);
}

Interpreter::~Interpreter() {
   delete ast;
}

ASTNode* Interpreter::parse() {
   ast = parser.parse();
   return ast;
}

void Interpreter::interpret() {
   ast->accept(*this); 
}

std::vector<Token>& Interpreter::get_tokens() {
   if (tokens.empty()) {
      tokens = lexer.tokenize();
   }

   parser.set_tokens(tokens);
   //parser.parse();
   /*
   ASTNode* ast = parser.expr();

   std::cout << ast->to_str() << std::endl;

   std::shared_ptr<Value> v = visit_binary_op((BinaryOp*)ast);
   std::cout << v->get_int() << std::endl;
   */
   return tokens;
}

std::string Interpreter::token_to_str(Token t) {
   return Token::type_to_str.at(t.type);
}

std::shared_ptr<Value> Interpreter::visit_literal_int(LiteralInt* node) {
   log("Visited literal int: " + node->to_str(), LOG_VERBOSE);

   auto v = std::make_shared<Value>(Value::Type::INT, node->value);
   return v;
}

std::shared_ptr<Value> Interpreter::visit_binary_op(BinaryOp* node) {
   log("Visited binary operation: " +node->to_str(), LOG_VERBOSE);

   std::shared_ptr<Value> left = node->left->accept(*this);
   std::shared_ptr<Value> right = node->right->accept(*this);

   // for now assume these are ints
   switch(node->op.type) {
      case TokenType::PLUS:
         return std::make_shared<Value>(Value::Type::INT, left->get_int() + right->get_int());
      case TokenType::MINUS:
         return std::make_shared<Value>(Value::Type::INT, left->get_int() - right->get_int());
      case TokenType::MULT:
         return std::make_shared<Value>(Value::Type::INT, left->get_int() * right->get_int());
      case TokenType::DIV:
         return std::make_shared<Value>(Value::Type::INT, left->get_int() / right->get_int());
      default:
         return std::make_shared<Value>(Value::Type::NIL);
   }
}

std::shared_ptr<Value> Interpreter::visit_unary_op(UnaryOp* node) {
   log("Visited unary op: " + node->to_str(), LOG_VERBOSE);

   std::shared_ptr<Value> expr = node->expr->accept(*this);

   switch (node->token.type) {
      case TokenType::PLUS:
         return std::make_shared<Value>(Value::Type::INT, +expr->get_int());
      case TokenType::MINUS:
         return std::make_shared<Value>(Value::Type::INT, -expr->get_int());
      default:
         std::cout << "Error" << std::endl;
   }
}

std::shared_ptr<Value> Interpreter::visit_statement_list(StatementList* node) {
   log("Visited statement list:" + node->to_str(), LOG_VERBOSE);

   for (auto n : node->statements) {
      n->accept(*this);
   }

   return nullptr;
}

std::shared_ptr<Value> Interpreter::visit_block(Block* node) {
   log("Visited block: " + node->to_str(), LOG_VERBOSE);

   enter_new_scope();
   node->statements->accept(*this);
   exit_scope();

   return nullptr;
}

std::shared_ptr<Value> Interpreter::visit_no_op(NoOp* node) {
   return nullptr;
}

std::shared_ptr<Value> Interpreter::visit_assignment(Assignment* node) {
   log("Visited assignment: " + node->to_str(), LOG_VERBOSE);

   // assign variable to value
   auto nv = node->target->accept(*this);
   if (node->make_new_var) {
      assign_or_insert_variable(node->destination->get_var_name(), nv);
   } else {
      assign_variable(node->destination->get_var_name(), nv);
   }
   
   return nullptr;
}

std::shared_ptr<Value> Interpreter::visit_variable(Variable* node) {
   log("Visited variable: " + node->to_str(), LOG_VERBOSE);
   
   // get and return value
   auto var = find_variable(node->get_var_name());

   if (var != nullptr) {
      return var;
   } else {
      std::cout << "error tried to get variable that doesnt exist" << std::endl;
   }
}

std::shared_ptr<Value> Interpreter::visit_function_decl(FunctionDecl* node) {
   log("Visited Function Declaration: " + node->to_str(), LOG_VERBOSE);

   return nullptr;
}

void Interpreter::print_variables() {
   log("VARIABLES:");
   auto p = current_scope;
   int num = 0;

   int prev_scope = log_scope;

   while (p != nullptr) {
      log_scope++;
      num++;
      log("SCOPE #" + num);
      log(p->to_str());
      p = p->get_parent();
   }

   log_scope = prev_scope;
}
