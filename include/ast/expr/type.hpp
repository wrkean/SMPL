#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"
#include <cstddef>

class TypeNode : public ExprNode {
public:
    TypeNode(Token type, size_t line);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }
    SmplType get_type() const override { return smpl_type; }
    size_t get_line() const override { return line; }

    Token type;
    ExprASTKind kind;
    SmplType smpl_type;
    size_t line;
};
