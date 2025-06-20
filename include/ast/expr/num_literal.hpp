#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "token/token.hpp"

class NumLitNode : public ExprNode {
public:
    NumLitNode(Token literal);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }

    Token literal;
    ExprASTKind kind;
};
