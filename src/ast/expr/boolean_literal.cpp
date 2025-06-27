#include "ast/expr/boolean_literal.hpp"
#include "smpl/types.hpp"
#include <iostream>

BooleanLiteral::BooleanLiteral(Token literal, size_t line)
    : literal(literal), kind(ExprASTKind::BooleanLiteral), line(line), type(SmplType::Boolean) { }

void BooleanLiteral::print() const {
    std::cout << literal.lexeme;
}
