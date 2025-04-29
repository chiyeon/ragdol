#pragma once

#include <string>
#include <unordered_map>

enum class TokenType {
   FLOAT,
   INTEGER,
   STRING,
   BOOLEAN,
   IDENTIFIER,
   NOT, NOT_EQ,
   PLUS, MINUS, MULT, DIV,
   PLUS_EQ, MINUS_EQ, MULT_EQ, DIV_EQ,
   GT, LT, GTE, LTE, EQ,
   COMMA,
   PERIOD,
   ASSIGN,
   FUNCTIONDECL,
   VAR,
   IF,
   WHILE,
   FOR,
   PRINT,
   LEFTPAREN,
   RIGHTPAREN,
   LEFTBRACKET,
   RIGHTBRACKET,
   LEFTBRACE,
   RIGHTBRACE,
   STATEMENTEND, /* semi colon or end of line */
   SINGLELINECOMMENT,
   ENDOFFILE
};

struct Token {
   TokenType type;
   std::string lexeme;

   int line;
   int column;
   double number_value;

   Token(TokenType type, std::string lexeme, int line, int column)
      : type(type)
      , lexeme(std::move(lexeme))
      , line(line)
      , column(column)
      {}

   std::string to_str();

   static const std::unordered_map<TokenType, std::string> type_to_str;
};
