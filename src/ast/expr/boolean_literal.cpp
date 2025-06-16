#include "ast/expr/boolean_literal.hpp"
#include <iostream>

BooleanLiteral::BooleanLiteral(Token literal)
    : literal(literal) { }

void BooleanLiteral::print() const {
    std::cout << literal.lexeme;
}
