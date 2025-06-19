#include "ast/stmt/expr_stmt.hpp"
#include <memory>

ExprStmt::ExprStmt(std::unique_ptr<ExprNode> expr)
    : expr(std::move(expr)), kind(StmtASTKind::ExprStmt) { }

void ExprStmt::print() const {
    expr->print();
}
