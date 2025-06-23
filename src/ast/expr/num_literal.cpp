#include "ast/expr/num_literal.hpp"
#include <iostream>

NumLitNode::NumLitNode(Token literal, size_t line)
    : literal(literal), kind(ExprASTKind::NumberLiteral), line(line) { }

void NumLitNode::print() const {
    std::cout << literal.lexeme;
}
