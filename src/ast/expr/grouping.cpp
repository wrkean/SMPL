#include "ast/expr/grouping.hpp"
#include "smpl/types.hpp"
#include <iostream>

GroupingExpr::GroupingExpr(std::unique_ptr<ExprNode> expr)
    : expr(std::move(expr))
{
    type = get_type();
}

void GroupingExpr::print() const {
    std::cout << "{";
    expr->print();
    std::cout << "}";
}

SmplType GroupingExpr::get_type() {
    return expr->get_type();
}
