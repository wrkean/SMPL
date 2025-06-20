#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "token/token.hpp"
#include <memory>
#include <vector>

class FuncCallNode : public ExprNode {
public:
    FuncCallNode(Token identifier, std::vector<std::unique_ptr<ExprNode>> args);
    void print() const override;
    ExprASTKind get_kind() const override { return kind; }

    Token identifier;
    std::vector<std::unique_ptr<ExprNode>> args;
    ExprASTKind kind;
};
