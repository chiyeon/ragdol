#pragma once

#include "token.h"
#include "value.h"

#include <vector>
#include <string>
#include <memory>

/*
 * File includes definitions for AST visitor & nodetypes
 *
 * Lexer class converts raw text to tokens, parser converts tokens to AST nodes
 */

struct Interpreter;

struct ASTNode {
   Token token;

   ASTNode(Token t) : token(t) {}
   virtual ~ASTNode() = default;

   virtual std::shared_ptr<Value> accept(Interpreter& visitor) = 0;

   virtual std::string to_str() {
      return token.lexeme;
   }
};

struct BinaryOp : public ASTNode {
   ASTNode* left;
   Token op;
   ASTNode* right;
   
   BinaryOp(Token t, ASTNode* l, Token o, ASTNode* r) : ASTNode(t), left(l), op(o), right(r) {}
   std::shared_ptr<Value> accept(Interpreter& visitor) override;

   std::string to_str() override {
      return "BinaryOperation<" + left->to_str() + op.lexeme + right->to_str() + ">";
   }
};

struct LiteralInt : public ASTNode {
   int value;

   LiteralInt(Token t, int value) : ASTNode(t), value(value) {}
   std::shared_ptr<Value> accept(Interpreter& visitor) override;
};

struct UnaryOp : public ASTNode {
   ASTNode* expr;

   UnaryOp(Token t, ASTNode* expr) : ASTNode(t), expr(expr) {}
   std::shared_ptr<Value> accept(Interpreter& visitor) override;
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
   std::shared_ptr<Value> accept(Interpreter& visitor) override;

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

   std::shared_ptr<Value> accept(Interpreter& visitor) override;
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
   bool make_new_var;

   Assignment(Token t, Variable* destination, Token op, ASTNode* target)
      : ASTNode(t), destination(destination), op(op), target(target), make_new_var(true)
      {}
   Assignment(Token t, Variable* destination, Token op, ASTNode* target, bool make_new_var)
      : ASTNode(t), destination(destination), op(op), target(target), make_new_var(make_new_var)
      {}
   std::shared_ptr<Value> accept(Interpreter& visitor) override;

   std::string to_str() override {
      return "Assignment<" + destination->to_str() + " = " + target->to_str() + ">";
   }
};

struct NoOp : public ASTNode {
   NoOp(Token t) : ASTNode(t) {}
   std::shared_ptr<Value> accept(Interpreter& visitor) override;

   std::string to_str() override {
      return "NoOp";
   }
};

template<typename ret>
struct ASTVisitor {
   /*
    * ASTVisitor is a template class for visitors that 
    * have custom logic to handle each node
    */

   /*
    * EXPRESSIONS (returns std::shared_ptr<Value> for instance)
    */
   virtual ret visit_binary_op(BinaryOp*) = 0;
   virtual ret visit_literal_int(LiteralInt*) = 0;
   virtual ret visit_unary_op(UnaryOp*) = 0;
   virtual ret visit_variable(Variable*) = 0;

   /*
    * STATEMENTS
    * dont necessarily return anything
    */
   virtual ret visit_block(Block*) = 0;
   virtual ret visit_assignment(Assignment*) = 0;
   virtual ret visit_no_op(NoOp*) = 0;
};
