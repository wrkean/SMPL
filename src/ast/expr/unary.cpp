#include "ast/stmt/unary.hpp"

UnaryNode::UnaryNode(std::unique_ptr<ExprNode> right)
    : right(std::move(right)) { }

void UnaryNode::print() const {
    right->print();
}
