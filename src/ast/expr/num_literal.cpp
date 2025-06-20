#include "ast/expr/num_literal.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

NumLitNode::NumLitNode(Token literal)
    : literal(literal), kind(ExprASTKind::NumberLiteral) { }

void NumLitNode::print() const {
    std::cout << literal.lexeme;
}
