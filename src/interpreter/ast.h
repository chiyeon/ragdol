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

   // maybe put somewhere else
   std::string indent(const std::string& str, int tabs = 1) {
      /*
       * indents multi line strings
       */
      std::string indent(tabs, '\t');
      std::string out;

      size_t start = 0;
      size_t end;

      // indent at newlines
      while ((end = str.find('\n', start)) != std::string::npos) {
         out += indent + str.substr(start, end - start + 1);
         start = end + 1;
      }

      if (start < str.size()) {
         // add remaining
         out += indent + str.substr(start); 
      }

      return out;
   }
};

struct BinaryOp : public ASTNode {
   ASTNode* left;
   Token op;
   ASTNode* right;
   
   BinaryOp(Token t, ASTNode* l, Token o, ASTNode* r) : ASTNode(t), left(l), op(o), right(r) {}
   ~BinaryOp() {
      delete left;
      delete right;
   }
   std::shared_ptr<Value> accept(Interpreter& visitor) override;

   std::string to_str() override {
      return "BinaryOperation<" + left->to_str() + op.lexeme + right->to_str() + ">";
   }
};

struct Literal : public ASTNode {
   std::shared_ptr<Value> value;

   Literal(Token t, std::shared_ptr<Value> value)
      : ASTNode(t), value(value)
   {}

   std::shared_ptr<Value> accept(Interpreter& visitor) override;
};

struct UnaryOp : public ASTNode {
   ASTNode* expr;

   UnaryOp(Token t, ASTNode* expr) : ASTNode(t), expr(expr) {}
   ~UnaryOp() {
      delete expr;
   }
   std::shared_ptr<Value> accept(Interpreter& visitor) override;
};

struct StatementList : public ASTNode {
   std::vector<ASTNode*> statements;

   StatementList(Token t) : ASTNode(t) {}
   StatementList(Token t, std::vector<ASTNode*>&& statements)
      : ASTNode(t), statements(std::move(statements))
      {}

   ~StatementList() {
      for (auto t : statements) {
         delete t;
      }
   }

   std::shared_ptr<Value> accept(Interpreter& visitor) override;

   std::string to_str() override {
      std::string out = "Statement List [\n";
      for (auto s : statements) {
         out += "\t" + s->to_str() + "\n";
      }
      out += "]";

      return indent(out);
   }
};

struct Block : public ASTNode {
   /*
    * Block e.g. "{ StatementList }"
    */
   StatementList* statements;

   Block(Token t, StatementList* statements = nullptr) : ASTNode(t), statements(statements) {}

   ~Block() {
      delete statements;
   }

   std::shared_ptr<Value> accept(Interpreter& visitor) override;

   std::string to_str() override {
      return "Block {\n" + indent(statements->to_str() + "\n}");
   }
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

   ~Assignment() {
      delete destination;
      delete target;
   }

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

struct FunctionDecl : public ASTNode {
   std::string name;
   std::vector<std::string> params;
   StatementList* body;

   FunctionDecl(Token t, const std::string& name, std::vector<std::string>&& params, StatementList* body)
      : ASTNode(t), name(name), params(std::move(params)), body(body)
      {}
   ~FunctionDecl() {
      delete body;
   }

   std::string to_str() override {
      std::string out = "FunctionDecl<" + name + ", " + std::to_string(params.size()) + " args> {\n";

      out += indent(body->to_str() + "\n}");

      return out;
   }

   std::shared_ptr<Value> accept(Interpreter& visitor) override;
};

struct FunctionCall : public ASTNode {
   std::string name;
   std::vector<ASTNode*> arguments;
   bool returns; // do we return a val or not
   //Variable* destination; // may be nullptr

   FunctionCall(Token t, const std::string& name, std::vector<ASTNode*>&& arguments)
      : ASTNode(t), name(name), arguments(std::move(arguments)), returns(false)
   {}
   FunctionCall(Token t, const std::string& name, std::vector<ASTNode*>&& arguments, bool returns)
      : ASTNode(t), name(name), arguments(std::move(arguments)), returns(returns)
   {}

   ~FunctionCall() {
      for (auto n : arguments) {
         delete n;
      }
   }

   std::string to_str() override {
      return "FunctionCall<" + name + ">";
   }

   std::shared_ptr<Value> accept(Interpreter& visitor) override;
};

struct ReturnStatement : public ASTNode {
   ASTNode* value;

   ReturnStatement(Token t, ASTNode* value)
      : ASTNode(t), value(value)
   {}
   ReturnStatement(Token t)
      : ASTNode(t), value(nullptr)
   {}

   ~ReturnStatement() {
      if (value != nullptr) delete value;
   }

   std::string to_str() override {
      return "Return: " + (value != nullptr ? value->to_str() : "Nothing");
   }

   std::shared_ptr<Value> accept(Interpreter& visitor) override;
};

struct IfStatement : public ASTNode {
   ASTNode* condition;
   ASTNode* body;
   ASTNode* else_body;

   IfStatement(Token t, ASTNode* condition, ASTNode* body, ASTNode* else_body)
      : ASTNode(t), condition(condition), body(body), else_body(else_body)
   { }
   IfStatement(Token t, ASTNode* condition, ASTNode* body)
      : ASTNode(t), condition(condition), body(body), else_body(nullptr)
   { }

   void set_else(ASTNode* new_else) {
      if (else_body != nullptr) delete else_body;
      else_body = new_else;
   }

   ~IfStatement() {
      delete condition;
      delete body;
      if (else_body) delete else_body;
   }

   std::string to_str() override {
      return "If: (" + condition->to_str() + "): " + body->to_str() + ";"
      + (else_body != nullptr ? "else: " + else_body->to_str() : "");
   }

   std::shared_ptr<Value> accept(Interpreter& visitor) override;
};

struct WhileLoop : public ASTNode {
   ASTNode* condition;
   ASTNode* body;

   WhileLoop(Token t, ASTNode* condition, ASTNode* body)
      : ASTNode(t), condition(condition), body(body)
   {}

   ~WhileLoop() {
      delete condition;
      delete body;
   }

   std::string to_str() override {
      return "While: (" + condition->to_str() + "): " + body->to_str() + ";";
   }

   std::shared_ptr<Value> accept(Interpreter& visitor) override;
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
   virtual ret visit_literal(Literal*) = 0;
   virtual ret visit_unary_op(UnaryOp*) = 0;
   virtual ret visit_variable(Variable*) = 0;
   virtual ret visit_return(ReturnStatement*) = 0;

   /*
    * STATEMENTS
    * dont necessarily return anything
    */
   virtual ret visit_statement_list(StatementList*) = 0;
   virtual ret visit_block(Block*) = 0;
   virtual ret visit_assignment(Assignment*) = 0;
   virtual ret visit_no_op(NoOp*) = 0;
   virtual ret visit_function_decl(FunctionDecl*) = 0;
   virtual ret visit_function_call(FunctionCall*) = 0;
   virtual ret visit_if_statement(IfStatement*) = 0;
   virtual ret visit_while_loop(WhileLoop*) = 0;
};
