#include "ast/expr/unary.hpp"
#include "token/token.hpp"
#include <iostream>

UnaryNode::UnaryNode(Token op, std::unique_ptr<ExprNode> right)
    : op(op), right(std::move(right)) { }

void UnaryNode::print() const {
    std::cout << "[" << op.lexeme << " ";
    right->print();
    std::cout << "]";
}
