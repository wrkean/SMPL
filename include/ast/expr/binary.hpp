#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "token/token.hpp"
#include <memory>

class BinaryExpr : public ExprNode {
public:
    BinaryExpr(Token op, std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }

    Token op;
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
    ExprASTKind kind;
};
