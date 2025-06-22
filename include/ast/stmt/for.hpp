#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include "token/token.hpp"
#include <memory>

class ForNode : public StmtNode {
public:
    ForNode(Token bind_var, std::unique_ptr<ExprNode> iterator, std::unique_ptr<StmtNode> block, size_t line);
    void print() const override;
    StmtASTKind get_kind() const override { return kind; }
    size_t get_line() const override { return line; }

    Token bind_var;
    std::unique_ptr<ExprNode> iterator;
    std::unique_ptr<StmtNode> block;
    StmtASTKind kind;
    size_t line;
};
