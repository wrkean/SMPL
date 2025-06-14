#include "ast/expr/grouping.hpp"
#include <iostream>

GroupingExpr::GroupingExpr(std::unique_ptr<ExprNode> expr)
    : expr(std::move(expr)) { }

void GroupingExpr::print() const {
    std::cout << "{";
    expr->print();
    std::cout << "}";
}
