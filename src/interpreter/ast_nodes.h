#pragma once

#include "token.h"
#include "value.h"

#include <iostream>
#include <string>

/*
 * File includes definitions for AST visitor & nodetypes
 *
 * Lexer class converts raw text to tokens, parser converts tokens to AST nodes
 */

struct ASTVisitor;

struct ASTNode {
   Token token;

   ASTNode(Token t) : token(t) {}
   virtual ~ASTNode() = default;
   virtual Value* accept(ASTVisitor& visitor) = 0;

   virtual std::string to_str() {
      return "ASTNode";
   }
};

struct BinaryOp : public ASTNode {
   ASTNode* left;
   Token op;
   ASTNode* right;
   
   BinaryOp(Token t, ASTNode* l, Token o, ASTNode* r) : ASTNode(t), left(l), op(o), right(r) {}
   Value* accept(ASTVisitor& visitor) override;

   std::string to_str() {
      return "(" + left->to_str() + op.lexeme + right->to_str() + ")";
   }
};

struct LiteralInt : public ASTNode {
   int value;

   LiteralInt(Token t, int value) : ASTNode(t), value(value) {}
   Value* accept(ASTVisitor& visitor) override;

   std::string to_str() {
      return token.lexeme;
   }
};

struct ASTVisitor {
   virtual Value* visit_binary_op(BinaryOp*) = 0;
   virtual Value* visit_literal_int(LiteralInt*) = 0;
};
