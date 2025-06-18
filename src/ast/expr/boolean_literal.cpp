#include "ast/expr/boolean_literal.hpp"
#include "smpl/types.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

BooleanLiteral::BooleanLiteral(Token literal)
    : literal(literal)
{
    type = get_type();
}

void BooleanLiteral::print() const {
    std::cout << magic_enum::enum_name(type) << " " << literal.lexeme;
}

SmplType BooleanLiteral::get_type() {
    return SmplType::Boolean;
}
