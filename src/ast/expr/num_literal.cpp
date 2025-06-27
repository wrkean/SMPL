#include "ast/expr/num_literal.hpp"
#include "smpl/types.hpp"
#include <iostream>

NumLitNode::NumLitNode(Token literal, size_t line)
    : literal(literal), kind(ExprASTKind::NumberLiteral), line(line), type(SmplType::Unknown) { }

void NumLitNode::print() const {
    std::cout << literal.lexeme;
}
