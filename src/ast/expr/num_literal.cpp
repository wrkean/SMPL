#include "ast/expr/num_literal.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

NumLitNode::NumLitNode(Token literal, size_t line)
    : literal(literal), kind(ExprASTKind::NumberLiteral), line(line) { }

void NumLitNode::print() const {
    std::cout << literal.lexeme;
}
