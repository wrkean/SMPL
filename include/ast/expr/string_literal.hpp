#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"
#include <cstddef>

class StringLiteral : public ExprNode {
public:
    StringLiteral(const Token str_lit, size_t line);

    void print() const override;
    ExprASTKind get_kind() const override { return kind; }
    SmplType get_type() const override { return type; }
    size_t get_line() const override { return line; }

    Token literal;
    ExprASTKind kind;
    SmplType type;
    size_t line;
};
