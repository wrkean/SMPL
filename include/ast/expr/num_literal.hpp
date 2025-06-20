#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"

class NumLitNode : public ExprNode {
public:
    NumLitNode(Token literal);
    void print() const override;
    SmplType get_type() override;
    ExprASTKind get_kind() const override { return kind; }

    Token literal;
    SmplType type;
    ExprASTKind kind;
};
