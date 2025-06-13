#pragma once

#include "ast/expr.hpp"
#include "ast/visitor/visitor.hpp"
#include "token/token.hpp"

class NumLitNode : public ExprNode {
public:
    NumLitNode(Token literal);
    void accept(Visitor& visitor) override;

    Token literal;
};
