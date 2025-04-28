#include "token.h"

#include <string>
#include <unordered_map>

const std::unordered_map<TokenType, std::string> Token::type_to_str = {
   { TokenType::DOUBLE, "double" },
   { TokenType::INTEGER, "integer" },
   { TokenType::STRING, "string" },
   { TokenType::BOOLEAN, "boolean" },
   { TokenType::IDENTIFIER, "identifier" },
   { TokenType::NOT, "not" },
   { TokenType::NOT_EQ, "not_equal" },
   { TokenType::PLUS, "plus" },
   { TokenType::MINUS, "minus" },
   { TokenType::MULT, "mult" },
   { TokenType::DIV, "div" },
   { TokenType::PLUS_EQ, "plus_equals" },
   { TokenType::MINUS_EQ, "minus_equals" },
   { TokenType::MULT_EQ, "mult_equals" },
   { TokenType::DIV_EQ, "div_equals" },
   { TokenType::GT, "greater_than" },
   { TokenType::LT, "less_than" },
   { TokenType::LTE, "less_than_or_equal" },
   { TokenType::GTE, "greater_than_or_equal" },
   { TokenType::EQ, "equals" },
   { TokenType::ASSIGN, "assignment" },
   { TokenType::VAR, "var" },
   { TokenType::IF, "if" },
   { TokenType::WHILE, "while" },
   { TokenType::FOR, "for" },
   { TokenType::PRINT, "print" },
   { TokenType::LEFTPAREN, "left_paren" },
   { TokenType::RIGHTPAREN, "right_paren" },
   { TokenType::LEFTBRACKET, "left_bracket" },
   { TokenType::RIGHTBRACKET, "right_bracket" },
   { TokenType::LEFTBRACE, "left_brace" },
   { TokenType::RIGHTBRACE, "right_brace" },
   { TokenType::ENDOFFILE, "end_of_file" },
   { TokenType::SINGLELINECOMMENT, "single_line_comment" },
   { TokenType::STATEMENTEND, "statement_end" },
   { TokenType::FUNCTIONDECL, "function_decl" },
};

std::string Token::to_str() {
   return "Token<" + Token::type_to_str.at(type) + ", \"" + lexeme + "\">";
}
