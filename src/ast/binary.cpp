#include "ast/binary.hpp"
#include "ast/visitor/visitor.hpp"

BinaryExpr::BinaryExpr(Token op, std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right)
    : op(op), left(std::move(left)), right(std::move(right)) { }

void BinaryExpr::accept(Visitor& visitor) {
    visitor.visit(*this);
}
