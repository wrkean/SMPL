#include "ast/stmt/for.hpp"
#include <iostream>

ForNode::ForNode(Token bind_var, std::unique_ptr<ExprNode> iterator, std::unique_ptr<StmtNode> block, size_t line)
    : bind_var(bind_var), iterator(std::move(iterator)), block(std::move(block)), kind(StmtASTKind::For), line(line) { }

void ForNode::print() const {
    std::cout << "For loop\n";
    std::cout << "Bind var: " << bind_var.lexeme << "\n";
    std::cout << "In: ";
    iterator->print();
    std::cout << "\n";
    std::cout << "Block: ";
    block->print();
    std::cout << "End for loop";
}
