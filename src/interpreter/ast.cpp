#include "ast.h"
#include "interpreter.h"

/*
 * NODE ACCEPT DEF
 */
Value* BinaryOp::accept(Interpreter& visitor) {
   return visitor.visit_binary_op(this);
}

Value* LiteralInt::accept(Interpreter& visitor) {
   return visitor.visit_literal_int(this);
}

Value* UnaryOp::accept(Interpreter& visitor) {
   return visitor.visit_unary_op(this);
}

Value* Block::accept(Interpreter& visitor) {
   return visitor.visit_block(this);
}

Value* Variable::accept(Interpreter& visitor) {
   return visitor.visit_variable(this);
}

Value* Assignment::accept(Interpreter& visitor) {
   return visitor.visit_assignment(this);
}

Value* NoOp::accept(Interpreter& visitor) {
   return visitor.visit_no_op(this);
}
