#include "ast/stmt/expr_stmt.hpp"
#include <memory>

ExprStmt::ExprStmt(std::unique_ptr<ExprNode> expr, size_t line)
    : expr(std::move(expr)), kind(StmtASTKind::ExprStmt), line(line) { }

void ExprStmt::print() const {
    expr->print();
}
