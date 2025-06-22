#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include "token/token.hpp"
#include <cstddef>
#include <memory>

class AssignmentNode : public StmtNode {
public:
    AssignmentNode(Token variable, Token var_type, std::unique_ptr<ExprNode> right, size_t line);
    void print() const override;
    StmtASTKind get_kind() const override { return kind; }
    size_t get_line() const override { return line; }

    Token variable;
    Token var_type;
    std::unique_ptr<ExprNode> right;
    StmtASTKind kind;
    size_t line;
};
