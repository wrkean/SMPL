#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include <memory>

class GroupingExpr : public ExprNode {
public:
    GroupingExpr(std::unique_ptr<ExprNode> expr);
    void print() const override;
    SmplType get_type() override;
    ExprASTKind get_kind() const override { return kind; }

    std::unique_ptr<ExprNode> expr;
    ExprASTKind kind;
};
