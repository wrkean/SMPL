#include "ast/expr/num_literal.hpp"
#include <iostream>

NumLitNode::NumLitNode(Token literal)
    : literal(literal) { }

void NumLitNode::print() const {
    std::cout << literal.lexeme;
}

