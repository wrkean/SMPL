#include "ast/expr/identifier.hpp"
#include <iostream>

IdentifierNode::IdentifierNode(Token identifier)
    : identifier(identifier) { }

void IdentifierNode::print() const {
    std::cout << identifier.lexeme;
}
