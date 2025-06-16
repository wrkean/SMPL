#pragma once

#include "ast/expr/expr.hpp"
#include "token/token.hpp"

class BooleanLiteral : public ExprNode {
public:
    BooleanLiteral(Token literal);
    void print() const override;

    Token literal;
};
