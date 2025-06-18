#include "ast/expr/grouping.hpp"
#include "smpl/types.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

GroupingExpr::GroupingExpr(std::unique_ptr<ExprNode> expr)
    : expr(std::move(expr))
{
    type = get_type();
}

void GroupingExpr::print() const {
    std::cout << magic_enum::enum_name(type) << " {";
    expr->print();
    std::cout << "}";
}

SmplType GroupingExpr::get_type() {
    return expr->get_type();
}
