#include "parser.h"
#include "token.h"
#include "ast.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

ASTNode* Parser::parse() {
   ASTNode* node = program();
   // checks
   return node;
}

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
   if (peek().type == type) {
      advance();
   } else {
      error(type);
   }
}

ASTNode* Parser::program() {
   // TODO more here
   return block(); 
}

ASTNode* Parser::block() {
   Token block_start = peek();
   eat(TokenType::LEFTBRACE);
   Block* block = new Block(block_start, statement_list());
   eat(TokenType::RIGHTBRACE);

   return block;
}

std::vector<ASTNode*> Parser::statement_list() {
   std::vector<ASTNode*> nodes;
   nodes.push_back(statement());

   while (peek().type == TokenType::SEMICOLON) {
      eat(TokenType::SEMICOLON);
      nodes.push_back(statement());
   }

   // TODO error check
   
   return nodes;
}

ASTNode* Parser::statement() {
   switch (peek().type) {
      default:
         return empty();
      case TokenType::LEFTBRACE:
         return block();
      case TokenType::LET:
         return assignment_statement();
   }
}

ASTNode* Parser::assignment_statement() {
   /*
    * LET IDENTIFIER ASSIGN/= Expr
    */

   // deal with LET keyword (TODO; maybe no let?)
   Token let_token = peek();
   eat(TokenType::LET);

   // get variable / handle identifier
   Variable* dest = variable();
   
   // assign 
   Token assign_token = peek();
   eat(TokenType::ASSIGN);

   // expression target
   ASTNode* target = expr();

   return new Assignment(let_token, dest, assign_token, target);
}

Variable* Parser::variable() {
   Variable* n = new Variable(peek());
   eat(TokenType::IDENTIFIER);
   return n;
}

ASTNode* Parser::empty() {
   return new NoOp(peek());
}

ASTNode* Parser::factor() {
   /*
    * factor = INTEGER | LEFTPAREN ASTNode RIGHTPAREN
    */
   Token t = peek();
   switch (t.type) {
      default:
         /* TODO move this somewhere else? */
         return variable();
         break;
      case TokenType::PLUS:
         eat(TokenType::PLUS);
         return new UnaryOp(t, factor());
      case TokenType::MINUS:
         eat(TokenType::MINUS);
         return new UnaryOp(t, factor());
      case TokenType::INTEGER:
         eat(TokenType::INTEGER);
         return new LiteralInt(t, std::atoi(t.lexeme.c_str()));
      case TokenType::LEFTPAREN:
         eat(TokenType::LEFTPAREN);
         ASTNode* val = expr();
         eat(TokenType::RIGHTPAREN);
         return val;
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
