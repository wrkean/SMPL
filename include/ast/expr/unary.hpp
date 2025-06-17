#pragma once

#include "ast/expr/expr.hpp"
#include "token/token.hpp"
#include <memory>
class UnaryNode : public ExprNode {
public:
    UnaryNode(Token op, std::unique_ptr<ExprNode> right);
    void print() const override;

    Token op;
    std::unique_ptr<ExprNode> right;
};
