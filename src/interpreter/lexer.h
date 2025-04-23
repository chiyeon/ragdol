#pragma once

#include "token.h"

#include <vector>
#include <string>
#include <unordered_map>

class Lexer {
   std::string src;
   size_t current;
   size_t start;
   int line;
   int column;
   Token* last_token;

   bool is_at_end() const;
   char peek() const;
   char peekpeek() const;
   char advance();
   bool match(char expected);
   bool skip_whitespace();
   void skip_until_newline();

   Token make_token(TokenType type);

   Token scan_token();
   Token number();
   Token identifier();

   Token parse_number();
   Token parse_identifier();

   std::unordered_map<std::string, TokenType> keywords = {
      { "if", TokenType::IF },
      { "while", TokenType::WHILE },
      { "for", TokenType::FOR },
      { "print", TokenType::PRINT },
      { "true", TokenType::BOOLEAN },
      { "false", TokenType::BOOLEAN }
   };

public:
   explicit Lexer(std::string src)
      : src(std::move(src))
      , current(0)
      , start(0)
      , line(1)
      , column(1)
      {}
   
   std::vector<Token> tokenize(); 
};
