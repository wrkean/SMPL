#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include <memory>

class GroupingExpr : public ExprNode {
public:
    GroupingExpr(std::unique_ptr<ExprNode> expr, size_t line);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }
    size_t get_line() const override { return line; }

    std::unique_ptr<ExprNode> expr;
    ExprASTKind kind;
    SmplType type;
    size_t line;
};
