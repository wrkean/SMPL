#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/stmt/stmt.hpp"
#include <memory>
#include <vector>

class BlockNode : public StmtNode {
public:
    BlockNode(std::vector<std::unique_ptr<StmtNode>> statements, size_t line);
    void print() const override;
    StmtASTKind get_kind() const override { return kind; }
    size_t get_line() const override { return line; }

    std::vector<std::unique_ptr<StmtNode>> statements;
    StmtASTKind kind;
    size_t line;
};
