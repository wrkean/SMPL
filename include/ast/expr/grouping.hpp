#pragma once

#include "ast/expr/expr.hpp"
#include <memory>

class GroupingExpr : public ExprNode {
public:
    GroupingExpr(std::unique_ptr<ExprNode> expr);
    void print() const override;

    std::unique_ptr<ExprNode> expr;
};
