#pragma once

#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include <memory>

class ReturnNode : public StmtNode {
public:
    ReturnNode(std::unique_ptr<ExprNode> return_expr);
    void print() const override;

    std::unique_ptr<ExprNode> return_expr;
};
