#include "ast/stmt/while.hpp"
#include <iostream>

WhileNode::WhileNode(std::unique_ptr<ExprNode> condition, std::unique_ptr<StmtNode> block)
    :  condition(std::move(condition)), block(std::move(block)), kind(StmtASTKind::While) { }

void WhileNode::print() const {
    std::cout << "While loop\n";
    std::cout << "Condition: ";
    condition->print();
    std::cout << "\n";
    std::cout << "Block: ";
    block->print();
    std::cout << "End while loop";
}
