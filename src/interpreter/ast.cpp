#include "ast.h"

Value* BinaryOp::accept(ASTVisitor& visitor) {
   return visitor.visit_binary_op(this);
}

Value* LiteralInt::accept(ASTVisitor& visitor) {
   return visitor.visit_literal_int(this);
}

Value* UnaryOp::accept(ASTVisitor& visitor) {
   return visitor.visit_unary_op(this);
}

Value* Block::accept(ASTVisitor& visitor) {
   return visitor.visit_block(this);
}

Value* Variable::accept(ASTVisitor& visitor) {
   return visitor.visit_variable(this);
}

Value* Assignment::accept(ASTVisitor& visitor) {
   return visitor.visit_assignment(this);
}

Value* NoOp::accept(ASTVisitor& visitor) {
   return visitor.visit_no_op(this);
}
