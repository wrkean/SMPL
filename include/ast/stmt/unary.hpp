#pragma once

#include "ast/expr/expr.hpp"
#include <memory>
class UnaryNode : public ExprNode {
public:
    UnaryNode(std::unique_ptr<ExprNode> right);
    void print() const override;

    std::unique_ptr<ExprNode> right;
};
