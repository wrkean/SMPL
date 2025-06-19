#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"
#include <memory>

class BinaryExpr : public ExprNode {
public:
    BinaryExpr(Token op, std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right);
    void print() const override;
    SmplType get_type() override;

    Token op;
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
    SmplType type;
    ExprASTKind kind;
};
