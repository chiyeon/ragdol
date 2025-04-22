#include "ast_nodes.h"

Value* BinaryOp::accept(ASTVisitor& visitor) {
   return visitor.visit_binary_op(this);
}

Value* LiteralInt::accept(ASTVisitor& visitor) {
   return visitor.visit_literal_int(this);
}
