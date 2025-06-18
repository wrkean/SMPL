#include "ast/expr/identifier.hpp"
#include "smpl/types.hpp"
#include <iostream>

IdentifierNode::IdentifierNode(Token identifier)
    : identifier(identifier)
{
    type = get_type();
}

void IdentifierNode::print() const {
    std::cout << identifier.lexeme;
}

SmplType IdentifierNode::get_type() {
    // Can't be deduced yet
    return SmplType::Unknown;
}
