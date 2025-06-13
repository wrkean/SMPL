#include "ast/num_literal.hpp"
#include "ast/visitor/visitor.hpp"

NumLitNode::NumLitNode(Token literal)
    : literal(literal) { }

void NumLitNode::accept(Visitor& visitor) {
    visitor.visit(*this);
}

