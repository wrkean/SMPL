#include "ast/expr/identifier.hpp"
#include <iostream>

IdentifierNode::IdentifierNode(Token identifier, size_t line)
    : identifier(identifier), kind(ExprASTKind::Identifier), line(line) { }

void IdentifierNode::print() const {
    std::cout << identifier.lexeme;
}
