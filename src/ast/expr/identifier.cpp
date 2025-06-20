#include "ast/expr/identifier.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

IdentifierNode::IdentifierNode(Token identifier)
    : identifier(identifier), kind(ExprASTKind::Identifier) { }

void IdentifierNode::print() const {
    std::cout << identifier.lexeme;
}
