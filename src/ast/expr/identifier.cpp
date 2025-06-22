#include "ast/expr/identifier.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

IdentifierNode::IdentifierNode(Token identifier, size_t line)
    : identifier(identifier), kind(ExprASTKind::Identifier), line(line) { }

void IdentifierNode::print() const {
    std::cout << identifier.lexeme;
}
