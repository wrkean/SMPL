#include "ast/expr/string_literal.hpp"
#include <iostream>

StringLiteral::StringLiteral(Token str_lit, size_t line)
    : literal(str_lit), line(line), kind(ExprASTKind::StringLiteral) { }

void StringLiteral::print() const {
    std::cout << literal.lexeme;
}
