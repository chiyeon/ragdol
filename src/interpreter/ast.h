#pragma once

#include "token.h"
#include "value.h"

#include <vector>
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
      return token.lexeme;
   }
};

struct BinaryOp : public ASTNode {
   ASTNode* left;
   Token op;
   ASTNode* right;
   
   BinaryOp(Token t, ASTNode* l, Token o, ASTNode* r) : ASTNode(t), left(l), op(o), right(r) {}
   Value* accept(ASTVisitor& visitor) override;

   std::string to_str() override {
      return "BinaryOperation<" + left->to_str() + op.lexeme + right->to_str() + ">";
   }
};

struct LiteralInt : public ASTNode {
   int value;

   LiteralInt(Token t, int value) : ASTNode(t), value(value) {}
   Value* accept(ASTVisitor& visitor) override;
};

struct UnaryOp : public ASTNode {
   ASTNode* expr;

   UnaryOp(Token t, ASTNode* expr) : ASTNode(t), expr(expr) {}
   Value* accept(ASTVisitor& visitor) override;
};

struct Block : public ASTNode {
   /*
    * Block e.g. "{ ... }"
    */
   std::vector<ASTNode*> statements;

   Block(Token t) : ASTNode(t) {}
   /* todo use std::move for speed */
   Block(Token t, const std::vector<ASTNode*>& statements)
      : ASTNode(t), statements(statements)
      {}
   Value* accept(ASTVisitor& visitor) override;

   std::string to_str() override {
      std::string out = "Block {\n";
      for (auto s : statements) {
         out += "\t" + s->to_str() + "\n";
      }
      out += "}";

      return out;
   }

   /*
   void set_statements(std::vector<ASTNode*>& statements) {
      statements = statements;
   }
   */
};

struct Variable : public ASTNode {
   /*
    * variable obj
    */
   std::string var_name; // this is just token.lexeme but we store it here to make more sense in AST

   Variable(Token t)
      : ASTNode(t), var_name(t.lexeme)
      {}

   Value* accept(ASTVisitor& visitor) override;
   std::string get_var_name() { return var_name; }

   std::string to_str() {
      return "Variable<" + var_name + ">";
   }
};

struct Assignment : public ASTNode {
   /*
    * variable assignment
    */
   Variable* destination;
   Token op;
   ASTNode* target;

   Assignment(Token t, Variable* destination, Token op, ASTNode* target)
      : ASTNode(t), destination(destination), op(op), target(target)
      {}
   Value* accept(ASTVisitor& visitor) override;

   std::string to_str() override {
      return "Assignment<" + destination->to_str() + " = " + target->to_str() + ">";
   }
};

struct NoOp : public ASTNode {
   NoOp(Token t) : ASTNode(t) {}
   Value* accept(ASTVisitor& visitor) override;

   std::string to_str() override {
      return "NoOp";
   }
};

struct ASTVisitor {
   virtual Value* visit_binary_op(BinaryOp*) = 0;
   virtual Value* visit_literal_int(LiteralInt*) = 0;
   virtual Value* visit_unary_op(UnaryOp*) = 0;
   virtual Value* visit_block(Block*) = 0;
   virtual Value* visit_variable(Variable*) = 0;
   virtual Value* visit_assignment(Assignment*) = 0;
   virtual Value* visit_no_op(NoOp*) = 0;
};
