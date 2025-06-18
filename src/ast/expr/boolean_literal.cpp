#include "ast/expr/boolean_literal.hpp"
#include "smpl/types.hpp"
#include <iostream>

BooleanLiteral::BooleanLiteral(Token literal)
    : literal(literal)
{
    type = get_type();
}

void BooleanLiteral::print() const {
    std::cout << literal.lexeme;
}

SmplType BooleanLiteral::get_type() {
    return SmplType::Boolean;
}
