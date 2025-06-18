#include "ast/expr/identifier.hpp"
#include "smpl/types.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

IdentifierNode::IdentifierNode(Token identifier)
    : identifier(identifier)
{
    type = get_type();
}

void IdentifierNode::print() const {
    std::cout << magic_enum::enum_name(type) << " " << identifier.lexeme;
}

SmplType IdentifierNode::get_type() {
    // Can't be deduced yet
    return SmplType::Unknown;
}
