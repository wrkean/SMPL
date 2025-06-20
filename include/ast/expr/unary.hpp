#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"
#include <memory>
class UnaryNode : public ExprNode {
public:
    UnaryNode(Token op, std::unique_ptr<ExprNode> right);
    void print() const override;
    SmplType get_type() override;
    ExprASTKind get_kind() const override { return kind; }

    Token op;
    std::unique_ptr<ExprNode> right;
    SmplType type;
    ExprASTKind kind;
};
