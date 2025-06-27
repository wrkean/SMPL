#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"
#include <memory>

class BinaryExpr : public ExprNode {
public:
    BinaryExpr(Token op, std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right, size_t line);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }
    SmplType get_type() const override { return type; }
    size_t get_line() const override { return line; }

    Token op;
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
    ExprASTKind kind;
    SmplType type;
    size_t line;
};
