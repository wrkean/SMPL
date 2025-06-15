#pragma once

#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include "token/token.hpp"
#include <memory>

class AssignmentNode : public StmtNode {
public:
    AssignmentNode(Token variable, Token var_type, Token op, std::unique_ptr<ExprNode> right);
    void print() const override;

    Token variable;
    Token var_type;
    Token op;
    std::unique_ptr<ExprNode> right;
};
