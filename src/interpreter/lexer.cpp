#include "lexer.h"

#include <string>
#include <vector>
#include <iostream>

bool Lexer::is_at_end() const {
   return current >= src.size();
}

char Lexer::peek() const {
   return current < src.size() ? src[current] : '\0';
}

char Lexer::peekpeek() const {
   return current + 1 < src.size() ? src[current+1] : '\0';
}

char Lexer::advance() {
   column++;
   return src[current++]; 
}

bool Lexer::match(char expected) {
   if (is_at_end() || src[current] != expected) return false;
   current++;
   column++;
   return true;
}

void Lexer::skip_whitespace() {
   while (!is_at_end()) {
      char c = peek();
      if (std::isspace(c)) {
         start++;
         if (c == '\n') {
            line++;
            column = 1;
         } else {
            column++;
         }
         advance();
      } else {
         break;
      }
   }
}

void Lexer::skip_until_newline() {
   /*
    * skips all following characters in src
    * until a newline is found (inclusive)
    * for single line comments
    */
   while (!is_at_end()) {
      char c = peek();
      if (c == '\n') {
         return;
      } else {
         c = advance();
      }
   }
}

Token Lexer::make_token(TokenType type) {
   return Token(type, src.substr(start, current - start), line, column - (current - start));
}

Token Lexer::scan_token() {
   // go to next valid thing to lex
   skip_whitespace();

   // get current char & advance pos.
   // src[current] is now the next char
   char c = advance();

   // could be binary op or comp
   switch (c) {
      case '=': return match('=') ? make_token(TokenType::EQ) : make_token(TokenType::ASSIGN); break;
      case '>': return match('=') ? make_token(TokenType::GTE) : make_token(TokenType::GT); break;
      case '<': return match('=') ? make_token(TokenType::LTE) : make_token(TokenType::LT); break;
      case '!': return match('=') ? make_token(TokenType::NOT_EQ) : make_token(TokenType::NOT); break;
      case '+': return match('=') ? make_token(TokenType::PLUS_EQ) : make_token(TokenType::PLUS); break;
      case '-': return match('=') ? make_token(TokenType::MINUS_EQ) : make_token(TokenType::MINUS); break;
      case '*': return match('=') ? make_token(TokenType::MULT_EQ) : make_token(TokenType::MULT); break;
      case '/': return match('=') ? make_token(TokenType::DIV_EQ) : make_token(TokenType::DIV); break;
      case '(': return make_token(TokenType::LEFTPAREN); break;
      case ')': return make_token(TokenType::RIGHTPAREN); break;
      case '[': return make_token(TokenType::LEFTBRACKET); break;
      case ']': return make_token(TokenType::RIGHTBRACKET); break;
      case '{': return make_token(TokenType::LEFTBRACE); break;
      case '}': return make_token(TokenType::RIGHTBRACE); break;
      case ';': return make_token(TokenType::SEMICOLON); break;
      case '#': 
         Token comment = make_token(TokenType::SINGLELINECOMMENT);
         return comment;
         break;
   }

   // literals & identifiers
   if (isdigit(c)) return parse_number();
   if (isalpha(c) || c == '_') return parse_identifier();
}

Token Lexer::parse_number() {
   while (isdigit(peek())) advance();

   if (peek() == '.' && isdigit(peekpeek())) {
      advance();
      while (isdigit(peek())) advance();
      return make_token(TokenType::DOUBLE);
   }

   return make_token(TokenType::INTEGER);
}

Token Lexer::parse_identifier() {
   // build start -> current as a whole string (store to text)
   while (isalnum(peek()) || peek() == '_') advance();
   std::string text = src.substr(start, current - start);

   // get corresponding TokenType based on text if its a kw
   auto it = keywords.find(text);
   if (it != keywords.end()) {
      // found, create valid token
      return make_token(it->second);
   }

   //otherwise must be identifier
   return make_token(TokenType::IDENTIFIER);
}

std::vector<Token> Lexer::tokenize() {
   std::vector<Token> tokens;
   while (!is_at_end()) {
      start = current;
      Token t = scan_token();
      std::cout << "found " << t.to_str() << std::endl;

      switch (t.type) {
         default:
            tokens.push_back(t);
         case TokenType::ENDOFFILE:
            break;
         case TokenType::SINGLELINECOMMENT:
            skip_until_newline();
            break;
      }
   }

   tokens.emplace_back(TokenType::ENDOFFILE, "EOF", line, column);

   return tokens;
}
