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
   return current >= tokens.size();
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

   //std::cout << "Ended because enconutered " << peek().to_str() << std::endl;

   // TODO error check
   
   return new StatementList(start, std::move(nodes));
}

ASTNode* Parser::statement() {
   //std::cout << "new statement starting line " << peek().line << std::endl;
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
      case TokenType::RETURN:
         return return_statement();
      case TokenType::IF:
         return control_statement();
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
   ASTNode* target = full_expr();

   return new Assignment(t, dest, assign_token, target, new_var);
}

ASTNode* Parser::control_statement() {
   Token t = peek();

   ASTNode* head;
   ASTNode* p;

   // get normal if statmenet first
   // a little verbose this approach
   // but its clearer and more rock solid
   eat(TokenType::IF);

   // get condition
   eat(TokenType::LEFTPAREN);
   ASTNode* condition = full_expr();
   eat(TokenType::RIGHTPAREN);

   ASTNode* body = block();

   head = new IfStatement(t, condition, body);
   p = head;

   // cycle through any elifs
   t = peek();
   while (t.type == TokenType::ELIF) {
      eat(TokenType::ELIF);

      // similar to before, just keep creating
      // but this time connect the previous with us now

      eat(TokenType::LEFTPAREN);
      ASTNode* condition = full_expr();
      eat(TokenType::RIGHTPAREN);

      ASTNode* body = block();

      // move pointer along, linking to the previous 
      // as an else clause
      ASTNode* n = new IfStatement(t, condition, body);
      ((IfStatement*)p)->set_else(n);
      p = n;

      t = peek();
   }

   // now that we've cycled thorugh all, check for any else
   // statements at the end, connecting to the last
   // in our linked list

   if (peek().type == TokenType::ELSE) {
      eat(TokenType::ELSE);
      ASTNode* else_body = block();

      ((IfStatement*) p)->set_else(else_body);
   }

   // return head
   return head;
}

ReturnStatement* Parser::return_statement() {
   /*
    * return EXPR?
    */

   Token t = peek();
   eat(TokenType::RETURN);

   // parse argument if there
   ASTNode* ret_value = nullptr;
   if (peek().type != TokenType::STATEMENTEND) {
      // try to parse argument
      ret_value = full_expr();
      //advance();

      
      // TODO replace this
      // just double check make sure the statement is ending
      if (peek().type != TokenType::STATEMENTEND) {
         std::cout << "ERROR: Expected statement end" << std::endl;
      }
      
   }

   return new ReturnStatement(t, ret_value);
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
   if (is_at_end()) {
      std::cout << "UH OH STINKY " << std::endl;
   }
   switch (t.type) {
      default:
         std::cout << "ERROR: Found type " << t.type_to_str.at(t.type) << " and didn't know what to do" << std::endl;
         /* TODO move this somewhere else? */
         //return variable();
         break;
      case TokenType::IDENTIFIER:
         if (peekpeek().type == TokenType::LEFTPAREN) {
            // function call
            return function_call(true);
         } else {
            // otherwise variable
            return variable();
         }
      case TokenType::PLUS:
         eat(TokenType::PLUS);
         return new UnaryOp(t, factor());
      case TokenType::MINUS:
         eat(TokenType::MINUS);
         return new UnaryOp(t, factor());
      case TokenType::BOOLEAN:
         eat(TokenType::BOOLEAN);
         return new Literal(t, Value::make(Value::Type::BOOL, t.lexeme == "true"));
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
         ASTNode* val = full_expr();
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

ASTNode* Parser::comparison() {
   ASTNode* n = expr();

   while (peek().type == TokenType::EQ || peek().type == TokenType::NOT_EQ || peek().type == TokenType::GT || peek().type == TokenType::GTE || peek().type == TokenType::LT || peek().type == TokenType::LTE) {
      Token t = peek();
      eat(t.type);

      n = new BinaryOp(t, n, t, expr());
   }

   return n;
}

ASTNode* Parser::logical_and() {
   ASTNode* n = comparison();

   while (peek().type == TokenType::LOGICAL_AND) {
      Token t = peek();
      eat(TokenType::LOGICAL_AND);

      n = new BinaryOp(t, n, t, comparison());
   }

   return n;
}

ASTNode* Parser::logical_or() {
   ASTNode* n = logical_and();

   while (peek().type == TokenType::LOGICAL_OR) {
      Token t = peek();
      eat(TokenType::LOGICAL_OR);

      n = new BinaryOp(t, n, t, logical_and());
   }

   return n;
}

ASTNode* Parser::full_expr() {
   return logical_or();
}

FunctionCall* Parser::function_call(bool returning) {
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

         args.push_back(full_expr());
         if (peek().type == TokenType::RIGHTPAREN) {
            break;
         } else {
            eat(TokenType::COMMA);
         }
      }
   }

   eat(TokenType::RIGHTPAREN);

   return new FunctionCall(start, fn_name, std::move(args), returning);
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
                  //std:: cout << "P" << c << std::endl;
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
