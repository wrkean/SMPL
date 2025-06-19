#include "ast/stmt/if.hpp"
#include <iostream>

IfNode::IfNode(std::vector<ConditionBlock> branches)
    : branches(std::move(branches)), kind(StmtASTKind::If) { }

void IfNode::print() const {
    std::cout << "If stmt\n";
    for (auto& branch : branches) {
        std::cout << "Condition: ";
        branch.condition->print();
        std::cout << "\n";
        std::cout << "Branch: ";
        branch.block->print();
    }
    std::cout << "End if stmt";
}
