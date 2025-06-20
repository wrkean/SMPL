#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include <memory>

class ReturnNode : public StmtNode {
public:
    ReturnNode(std::unique_ptr<ExprNode> return_expr);
    void print() const override;
    StmtASTKind get_kind() const override { return kind; }

    std::unique_ptr<ExprNode> return_expr;
    StmtASTKind kind;
};
