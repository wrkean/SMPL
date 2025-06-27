#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"

class NumLitNode : public ExprNode {
public:
    NumLitNode(Token literal, size_t line);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }
    size_t get_line() const override { return line; }

    Token literal;
    ExprASTKind kind;
    SmplType type;
    size_t line;
};
