#include "parser.h"
#include "token.h"
#include "ast.h"

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <set>

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

Token Parser::peekpeek() {
   return tokens.at(current + 1);
}

void Parser::eat(TokenType type) {
   if (peek().type == type) {
      advance();
   } else {
      error(type);
   }
}

ASTNode* Parser::program() {
   return statement_list();
}

ASTNode* Parser::block() {
   Token block_start = peek();
   eat(TokenType::LEFTBRACE);
   Block* block = new Block(block_start, statement_list());
   eat(TokenType::RIGHTBRACE);

   return block;
}

StatementList* Parser::statement_list() {
   Token start = peek();
   std::vector<ASTNode*> nodes;
   nodes.push_back(statement());

   while (peek().type == TokenType::STATEMENTEND) {
      eat(TokenType::STATEMENTEND);
      nodes.push_back(statement());
   }

   // TODO error check
   
   return new StatementList(start, std::move(nodes));
}

ASTNode* Parser::statement() {
   switch (peek().type) {
      default:
         return empty();
      case TokenType::LEFTBRACE:
         return block();
      case TokenType::VAR:
         return assignment_statement(true);
      case TokenType::IDENTIFIER:
         if (peekpeek().type == TokenType::LEFTPAREN) {
            return function_call();
         } else {
            return assignment_statement(false);
         }
      case TokenType::FUNCTIONDECL:
         return function_decl();
   }
}

ASTNode* Parser::assignment_statement(bool new_var) {
   /*
    * var IDENTIFIER ASSIGN/= Expr
    *
    * if new_var is false, instead:
    * IDENTIFIER ASSIGN/= Expr
    */
   Token t = peek();

   if (new_var) eat(TokenType::VAR);

   // get variable / handle identifier
   Variable* dest = variable();
   
   // assign 
   Token assign_token = peek();
   eat(TokenType::ASSIGN);

   // expression target
   ASTNode* target = expr();

   return new Assignment(t, dest, assign_token, target, new_var);
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
         return new Literal(t, Value::make(Value::Type::INT, std::stoi(t.lexeme)));
      case TokenType::FLOAT:
         eat(TokenType::FLOAT);
         return new Literal(t, Value::make(Value::Type::FLOAT, std::stof(t.lexeme)));
      case TokenType::STRING:
         eat(TokenType::STRING);
         return new Literal(t, Value::make(Value::Type::STRING, t.lexeme));
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

ASTNode* Parser::expr() {
   ASTNode* n = term();

   while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
      Token t = peek();
      eat(t.type);

      n = new BinaryOp(t, n, t, term());
   }

   return n;
}

FunctionCall* Parser::function_call() {
   /*
    * identifier aka fn name lparen arbitrary # of agrs rparen
    */

   std::string fn_name = peek().lexeme;
   std::vector<ASTNode*> args;
   Token start = peek();

   eat(TokenType::IDENTIFIER);
   eat(TokenType::LEFTPAREN);

   // get arbitrary number of arguments
   // these could literals, vars, expressions, etc so do expr()
   if (peek().type != TokenType::RIGHTPAREN) {
      Token next = peek();
      while (true) {
         next = peek();

         args.push_back(expr());
         if (peek().type == TokenType::RIGHTPAREN) {
            eat(TokenType::RIGHTPAREN);
            break;
         } else {
            eat(TokenType::COMMA);
         }
      }
   }

   return new FunctionCall(start, fn_name, std::move(args));
}

FunctionDecl* Parser::function_decl() {
   /*
    * fn IDENTIFIER(fn name) lparen, arbitrary # of args, r paren
    * block/ { statmeent list }
    */

   std::string name;
   std::vector<std::string> params;
   std::set<std::string> encountered_params; // used just to check for dupes
   StatementList* statements;

   Token start = peek();

   // eat function keyword
   eat(TokenType::FUNCTIONDECL);

   // get name of function
   name = peek().lexeme;
   eat(TokenType::IDENTIFIER);

   // arguments
   eat(TokenType::LEFTPAREN);
   Token next = peek();

   bool parsing_args = true;
   while (parsing_args) {
      next = peek();
      switch (next.type) {
         default:
            std::cout << "ERROR: Encountered unexpected token when parsing function arguments: " << Token::type_to_str.at(next.type) << std::endl;
            parsing_args = false;
            break;
         case TokenType::RIGHTPAREN:
            eat(TokenType::RIGHTPAREN);
            parsing_args = false;
            break;
         case TokenType::IDENTIFIER:
            // check for dupes
            if (encountered_params.contains(next.lexeme)) {
               for (auto c : encountered_params) {
                  std:: cout << "P" << c << std::endl;
               }
               parsing_args = false;
               std::cout << "ERROR: Encountered duplicate parameter name when parsing function arguements" << std::endl;
            } else {
               // if no dupes, just add
               params.push_back(next.lexeme);
               encountered_params.insert(next.lexeme);
               // if comma eat comma, otherwise
               // assume we are done and eat right paren and break
               eat(TokenType::IDENTIFIER);
               if (peek().type == TokenType::COMMA) {
                  eat(TokenType::COMMA);
               } else {
                  // assume we're done
                  eat(TokenType::RIGHTPAREN);
                  parsing_args = false;
               }
            }
            break;
      }
   }

   eat(TokenType::LEFTBRACE);
   statements = statement_list();
   eat(TokenType::RIGHTBRACE);

   return new FunctionDecl(start, name, std::move(params), statements);
}
