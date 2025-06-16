#pragma once

#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include <memory>

class WhileNode : public StmtNode {
public:
    WhileNode(std::unique_ptr<ExprNode> condition, std::unique_ptr<StmtNode> block);
    void print() const override;

    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<StmtNode> block;
};
