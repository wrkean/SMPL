#include "ast/expr/grouping.hpp"
#include <iostream>

GroupingExpr::GroupingExpr(std::unique_ptr<ExprNode> expr, size_t line)
    : expr(std::move(expr)), kind(ExprASTKind::Grouping), line(line) { }

void GroupingExpr::print() const {
    std::cout << "{";
    expr->print();
    std::cout << "}";
}
