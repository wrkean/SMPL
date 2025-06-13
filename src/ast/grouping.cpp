#include "ast/grouping.hpp"
#include "visitor/visitor.hpp"

GroupingExpr::GroupingExpr(std::unique_ptr<ExprNode> expr)
    : expr(std::move(expr)) { }

void GroupingExpr::accept(Visitor& visitor) {
    visitor.visit(*this);
}
