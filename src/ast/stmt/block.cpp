#include "ast/stmt/block.hpp"
#include "ast/stmt/stmt.hpp"
#include <iostream>

BlockNode::BlockNode(std::vector<std::unique_ptr<StmtNode>> statements)
    : statements(std::move(statements)) { }

void BlockNode::print() const {
    for (auto& stmt : statements) {
        stmt->print();
        std::cout << "\n";
    }
}
