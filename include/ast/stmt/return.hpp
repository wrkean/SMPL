#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include <memory>

class ReturnNode : public StmtNode {
public:
    ReturnNode(std::unique_ptr<ExprNode> return_expr, size_t line);
    void print() const override;
    StmtASTKind get_kind() const override { return kind; }
    size_t get_line() const override { return line; }

    std::unique_ptr<ExprNode> return_expr;
    StmtASTKind kind;
    size_t line;
};
