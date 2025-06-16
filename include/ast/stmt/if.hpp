#pragma once

#include "ast/stmt/condition_block.hpp"
#include "ast/stmt/stmt.hpp"
#include <vector>

class IfNode : public StmtNode {
public:
    IfNode(std::vector<ConditionBlock> branches);
    void print() const override;

    std::vector<ConditionBlock> branches;
};
