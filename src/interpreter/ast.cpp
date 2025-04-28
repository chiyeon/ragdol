#include "ast.h"
#include "interpreter.h"

/*
 * NODE ACCEPT DEF
 */
std::shared_ptr<Value> BinaryOp::accept(Interpreter& visitor) {
   return visitor.visit_binary_op(this);
}

std::shared_ptr<Value> LiteralInt::accept(Interpreter& visitor) {
   return visitor.visit_literal_int(this);
}

std::shared_ptr<Value> UnaryOp::accept(Interpreter& visitor) {
   return visitor.visit_unary_op(this);
}

std::shared_ptr<Value> Block::accept(Interpreter& visitor) {
   return visitor.visit_block(this);
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
