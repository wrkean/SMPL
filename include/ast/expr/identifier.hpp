#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"

class IdentifierNode : public ExprNode {
public:
    IdentifierNode(Token identifier);
    void print() const override;
    SmplType get_type() override;

    Token identifier;
    SmplType type;
    ExprASTKind kind;
};
