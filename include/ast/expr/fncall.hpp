#pragma once

#include "ast/expr/expr.hpp"
#include "token/token.hpp"
#include <memory>
#include <vector>

class FuncCallNode : public ExprNode {
public:
    FuncCallNode(Token identifier, std::vector<std::unique_ptr<ExprNode>> args);
    void print() const override;

    Token identifier;
    std::vector<std::unique_ptr<ExprNode>> args;
};
