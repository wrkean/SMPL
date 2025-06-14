#pragma once

#include "ast/expr/expr.hpp"
#include "token/token.hpp"

class NumLitNode : public ExprNode {
public:
    NumLitNode(Token literal);
    void print() const override;

    Token literal;
};
