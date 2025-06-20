#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "token/token.hpp"

class IdentifierNode : public ExprNode {
public:
    IdentifierNode(Token identifier);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }

    Token identifier;
    ExprASTKind kind;
};
