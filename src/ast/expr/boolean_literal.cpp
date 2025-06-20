#include "ast/expr/boolean_literal.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

BooleanLiteral::BooleanLiteral(Token literal)
    : literal(literal), kind(ExprASTKind::BooleanLiteral) { }

void BooleanLiteral::print() const {
    std::cout << literal.lexeme;
}
