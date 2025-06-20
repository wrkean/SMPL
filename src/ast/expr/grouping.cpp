#include "ast/expr/grouping.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

GroupingExpr::GroupingExpr(std::unique_ptr<ExprNode> expr)
    : expr(std::move(expr)), kind(ExprASTKind::Grouping) { }

void GroupingExpr::print() const {
    std::cout << "{";
    expr->print();
    std::cout << "}";
}
