#include "ast/stmt/while.hpp"
#include <iostream>

WhileNode::WhileNode(std::unique_ptr<ExprNode> condition, std::unique_ptr<StmtNode> block)
    :  condition(std::move(condition)), block(std::move(block)) { }

void WhileNode::print() const {
    std::cout << "Condition: ";
    condition->print();
    std::cout << "\n";
    std::cout << "Block: ";
    block->print();
}
