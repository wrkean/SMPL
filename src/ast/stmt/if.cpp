#include "ast/stmt/if.hpp"
#include <iostream>

IfNode::IfNode(std::vector<ConditionBlock> branches)
    : branches(std::move(branches)) { }

void IfNode::print() const {
    for (auto& branch : branches) {
        std::cout << "Condition: ";
        branch.condition->print();
        std::cout << "\n";
        std::cout << "Branch: ";
        branch.block->print();
    }
}
