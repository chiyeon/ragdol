#include "parser.h"
#include "token.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

bool Parser::is_at_end() {
   return tokens.at(current).type == TokenType::ENDOFFILE;
}

Token Parser::advance() {
   current++;
   return tokens.at(current - 1);
}

Token Parser::peek() {
   return tokens.at(current);
}

void Parser::eat(TokenType type) {
   std::cout << "Checking " << Token::type_to_str.at(type) << std::endl;
   if (peek().type == type) {
      advance();
   } else {
      error(type);
   }
}

ASTNode* Parser::factor() {
   /*
    * factor = INTEGER | LEFTPAREN ASTNode RIGHTPAREN
    */
   Token t = peek();
   if (t.type == TokenType::INTEGER) {
      eat(TokenType::INTEGER);
      return new LiteralInt(t, std::atoi(t.lexeme.c_str()));
   } else if (t.type == TokenType::LEFTPAREN) {
      eat(TokenType::LEFTPAREN);
      ASTNode* val = expr();
      eat(TokenType::RIGHTPAREN);
      return val;
   } else {
      error(TokenType::INTEGER);
   }
}

void Parser::error(TokenType expected_type) {
   std::cout << "ERROR at line " << peek().line << " column " << peek().column << ": Expected next token to be of type " << Token::type_to_str.at(expected_type) << " but was " << Token::type_to_str.at(peek().type) << std::endl;
   exit(1);
}

ASTNode* Parser::term() {
   /*
    * term = factor ((MUL | DIV) factor)*
    */

   ASTNode* n = factor();

   while (peek().type == TokenType::MULT || peek().type == TokenType::DIV) {
      Token t = peek();
      eat(t.type);

      n = new BinaryOp(peek(), n, t, factor());
   }

   return n;
}

LiteralInt* Parser::lower_literal_int() {
   Token t = advance();
   LiteralInt* li = new LiteralInt(t, std::atoi(t.lexeme.c_str()));

   return li;
}



ASTNode* Parser::expr() {
   ASTNode* n = term();

   while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
      Token t = peek();
      eat(t.type);

      n = new BinaryOp(t, n, t, term());
   }

   return n;
}
