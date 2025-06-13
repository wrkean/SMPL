#pragma once

#include "ast/expr.hpp"
#include "visitor/visitor.hpp"
#include <memory>

class GroupingExpr : public ExprNode {
public:
    GroupingExpr(std::unique_ptr<ExprNode> expr);
    void accept(Visitor& visitor) override;

    std::unique_ptr<ExprNode> expr;
};
