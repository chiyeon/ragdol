#include "interpreter.h"
#include "token.h"

#include <string>
#include <vector>

std::vector<Token>& Interpreter::get_tokens() {
   if (tokens.empty()) {
      tokens = lexer.tokenize();
   }

   parser.set_tokens(tokens);
   //parser.parse();
   ASTNode* ast = parser.expr();

   std::cout << ast->to_str() << std::endl;

   Value* v = visit_binary_op((BinaryOp*)ast);
   std::cout << v->get_int() << std::endl;

   return tokens;
}

std::string Interpreter::token_to_str(Token t) {
   return Token::type_to_str.at(t.type);
}

Value* Interpreter::visit_literal_int(LiteralInt* node) {
   Value* v = new Value(Value::Type::INT, node->value);
   return v;
}

Value* Interpreter::visit_binary_op(BinaryOp* node) {
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
