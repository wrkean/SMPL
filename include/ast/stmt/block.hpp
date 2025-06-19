#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/stmt/stmt.hpp"
#include <memory>
#include <vector>

class BlockNode : public StmtNode {
public:
    BlockNode(std::vector<std::unique_ptr<StmtNode>> statements);
    void print() const override;

    std::vector<std::unique_ptr<StmtNode>> statements;
    StmtASTKind kind;

};
