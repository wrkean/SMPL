#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include <memory>

class CondExprNode : public ExprNode {
public:
    CondExprNode(std::unique_ptr<ExprNode> if_val,
                 std::unique_ptr<ExprNode> if_expr,
                 std::unique_ptr<ExprNode> else_val,
                 size_t line);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }
    SmplType get_type() const override { return type; }
    size_t get_line() const override { return line; }

    std::unique_ptr<ExprNode> if_val;
    std::unique_ptr<ExprNode> if_expr;
    std::unique_ptr<ExprNode> else_val;
    ExprASTKind kind;
    SmplType type;
    size_t line;
};
