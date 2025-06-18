#pragma once

#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"

class BooleanLiteral : public ExprNode {
public:
    BooleanLiteral(Token literal);
    void print() const override;
    SmplType get_type() override;

    Token literal;
    SmplType type;
};
