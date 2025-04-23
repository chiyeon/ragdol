#include "interpreter.h"
#include "token.h"

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

Interpreter::Interpreter(std::string src)
   : lexer(src)
{
   tokens = lexer.tokenize();

   parser.set_tokens(tokens);
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

   Value* v = visit_binary_op((BinaryOp*)ast);
   std::cout << v->get_int() << std::endl;
   */
   return tokens;
}

std::string Interpreter::token_to_str(Token t) {
   return Token::type_to_str.at(t.type);
}

Value* Interpreter::visit_literal_int(LiteralInt* node) {
   if (debug_log) std::cout << "Visited literal int: " << node->to_str() << std::endl;

   Value* v = new Value(Value::Type::INT, node->value);
   return v;
}

Value* Interpreter::visit_binary_op(BinaryOp* node) {
   if (debug_log) std::cout << "Visited binary operation: " << node->to_str() << std::endl;

   Value* left = node->left->accept(*this);
   Value* right = node->right->accept(*this);

   // for now assume these are ints
   switch(node->op.type) {
      case TokenType::PLUS:
         return new Value(Value::Type::INT, left->get_int() + right->get_int());
      case TokenType::MINUS:
         return new Value(Value::Type::INT, left->get_int() - right->get_int());
      case TokenType::MULT:
         return new Value(Value::Type::INT, left->get_int() * right->get_int());
      case TokenType::DIV:
         return new Value(Value::Type::INT, left->get_int() / right->get_int());
      default:
         return new Value(Value::Type::NIL);
   }
}

Value* Interpreter::visit_unary_op(UnaryOp* node) {
   if (debug_log) std::cout << "Visited unary op: " << node->to_str() << std::endl;

   Value* expr = node->expr->accept(*this);

   switch (node->token.type) {
      case TokenType::PLUS:
         return new Value(Value::Type::INT, +expr->get_int());
      case TokenType::MINUS:
         return new Value(Value::Type::INT, -expr->get_int());
      default:
         std::cout << "Error" << std::endl;
   }
}

Value* Interpreter::visit_block(Block* node) {
   if (debug_log) std::cout << "Visited Block: " << node->to_str() << std::endl;

   for (auto n : node->statements) {
      n->accept(*this);
   }
   return nullptr;
}

Value* Interpreter::visit_no_op(NoOp* node) {
   if (debug_log) std::cout << "Visited NoOp" << std::endl;
   return nullptr;
}

Value* Interpreter::visit_assignment(Assignment* node) {
   if (debug_log) std::cout << "Visited Assignment: " << node->to_str() << std::endl;

   // assign variable to value
   Value* nv = node->target->accept(*this);
   variables.insert_or_assign(node->destination->get_var_name(), nv); 

   return nullptr;
}

Value* Interpreter::visit_variable(Variable* node) {
   if (debug_log) std::cout << "Visited Variable: " << node->to_str() << std::endl;
   
   // get and return value
   auto var = variables.find(node->get_var_name());

   if (var != variables.end()) {
      return var->second;
   } else {
      std::cout << "error tried to get variable that doesnt exist" << std::endl;
   }

   return nullptr;
}

void Interpreter::print_variables() {
   std::cout << "VARIABLES:" << std::endl;
   for (const auto& [var_name, value] : variables) {
      std::cout << "\t" + var_name + "\t = " << value->to_str() << std::endl;
   }
}
