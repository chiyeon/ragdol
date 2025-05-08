#include "ast.h"
#include "interpreter.h"

/*
 * NODE ACCEPT DEF
 */
std::shared_ptr<Value> BinaryOp::accept(Interpreter& visitor) {
   return visitor.visit_binary_op(this);
}

std::shared_ptr<Value> Literal::accept(Interpreter& visitor) {
   return visitor.visit_literal(this);
}

std::shared_ptr<Value> UnaryOp::accept(Interpreter& visitor) {
   return visitor.visit_unary_op(this);
}

std::shared_ptr<Value> Block::accept(Interpreter& visitor) {
   return visitor.visit_block(this);
}

std::shared_ptr<Value> ReturnStatement::accept(Interpreter& visitor) {
   return visitor.visit_return(this);
}

std::shared_ptr<Value> StatementList::accept(Interpreter& visitor) {
   return visitor.visit_statement_list(this);
}

std::shared_ptr<Value> Variable::accept(Interpreter& visitor) {
   return visitor.visit_variable(this);
}

std::shared_ptr<Value> Assignment::accept(Interpreter& visitor) {
   return visitor.visit_assignment(this);
}

std::shared_ptr<Value> NoOp::accept(Interpreter& visitor) {
   return visitor.visit_no_op(this);
}

std::shared_ptr<Value> FunctionDecl::accept(Interpreter& visitor) {
   return visitor.visit_function_decl(this);
}

std::shared_ptr<Value> FunctionCall::accept(Interpreter& visitor) {
   return visitor.visit_function_call(this);
}

std::shared_ptr<Value> IfStatement::accept(Interpreter& visitor) {
   return visitor.visit_if_statement(this);
}

std::shared_ptr<Value> WhileLoop::accept(Interpreter& visitor) {
   return visitor.visit_while_loop(this);
}
