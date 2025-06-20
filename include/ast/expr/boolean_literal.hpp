#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "token/token.hpp"

class BooleanLiteral : public ExprNode {
public:
    BooleanLiteral(Token literal);
    void print() const override;
    SmplType get_type() override;
    ExprASTKind get_kind() const override { return kind; }

    Token literal;
    ExprASTKind kind;
};
