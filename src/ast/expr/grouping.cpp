#include "ast/expr/grouping.hpp"
#include "smpl/types.hpp"
#include <iostream>

GroupingExpr::GroupingExpr(std::unique_ptr<ExprNode> expr, size_t line)
    : expr(std::move(expr)), kind(ExprASTKind::Grouping), line(line), type(SmplType::Unknown) { }

void GroupingExpr::print() const {
    std::cout << "{";
    expr->print();
    std::cout << "}";
}
