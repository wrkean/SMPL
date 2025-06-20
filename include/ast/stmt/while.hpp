#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include <memory>

class WhileNode : public StmtNode {
public:
    WhileNode(std::unique_ptr<ExprNode> condition, std::unique_ptr<StmtNode> block);
    void print() const override;
    StmtASTKind get_kind() const override { return kind; }

    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<StmtNode> block;
    StmtASTKind kind;
};
