#pragma once

#include "ast/expr.hpp"
#include "token/token.hpp"
#include <memory>

class BinaryExpr : public ExprNode {
public:
    BinaryExpr(Token op, std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right);
    void accept(Visitor& visitor) override;

    Token op;
    std::unique_ptr<ExprNode> left;
    std::unique_ptr<ExprNode> right;
};
