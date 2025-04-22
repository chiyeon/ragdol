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
