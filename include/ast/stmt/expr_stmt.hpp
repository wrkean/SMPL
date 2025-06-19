#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include <memory>

class ExprStmt : public StmtNode {
public:
    ExprStmt(std::unique_ptr<ExprNode> expr);
    void print() const override;

    std::unique_ptr<ExprNode> expr;
    StmtASTKind kind;
};
