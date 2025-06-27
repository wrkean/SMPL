#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"
#include <memory>
#include <vector>

class FuncCallNode : public ExprNode {
public:
    FuncCallNode(Token identifier, std::vector<std::unique_ptr<ExprNode>> args, size_t line);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }
    size_t get_line() const override { return line; }

    Token identifier;
    std::vector<std::unique_ptr<ExprNode>> args;
    ExprASTKind kind;
    SmplType type;
    size_t line;
};
