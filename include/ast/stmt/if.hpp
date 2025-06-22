#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/stmt/condition_block.hpp"
#include "ast/stmt/stmt.hpp"
#include <vector>

class IfNode : public StmtNode {
public:
    IfNode(std::vector<ConditionBlock> branches, size_t line);
    void print() const override;
    StmtASTKind get_kind() const override { return kind; }
    size_t get_line() const override { return line; }

    std::vector<ConditionBlock> branches;
    StmtASTKind kind;
    size_t line;
};
