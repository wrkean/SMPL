#include "ast/expr/unary.hpp"
#include "token/token.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

UnaryNode::UnaryNode(Token op, std::unique_ptr<ExprNode> right, size_t line)
    : op(op), right(std::move(right)), kind(ExprASTKind::Unary), line(line) { }

void UnaryNode::print() const {
    std::cout << " [" << op.lexeme << " ";
    right->print();
    std::cout << "]";
}
