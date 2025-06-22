#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "token/token.hpp"
#include <memory>
class UnaryNode : public ExprNode {
public:
    UnaryNode(Token op, std::unique_ptr<ExprNode> right, size_t line);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }
    size_t get_line() const override { return line; }

    Token op;
    std::unique_ptr<ExprNode> right;
    ExprASTKind kind;
    size_t line;
};
