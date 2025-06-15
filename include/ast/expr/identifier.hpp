#pragma once

#include "ast/expr/expr.hpp"
#include "token/token.hpp"

class IdentifierNode : public ExprNode {
public:
    IdentifierNode(Token identifier);
    void print() const override;

    Token identifier;
};
