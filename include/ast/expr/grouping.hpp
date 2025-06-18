#pragma once

#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include <memory>

class GroupingExpr : public ExprNode {
public:
    GroupingExpr(std::unique_ptr<ExprNode> expr);
    void print() const override;
    SmplType get_type() override;

    std::unique_ptr<ExprNode> expr;
    SmplType type;
};
