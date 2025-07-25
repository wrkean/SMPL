#include "ast/expr/identifier.hpp"
#include "smpl/types.hpp"
#include <iostream>

IdentifierNode::IdentifierNode(Token identifier, size_t line)
    : identifier(identifier), kind(ExprASTKind::Identifier), line(line), type(SmplType::Unknown) { }

void IdentifierNode::print() const {
    std::cout << identifier.lexeme;
}
