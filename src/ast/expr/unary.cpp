#include "ast/expr/unary.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"
#include <iostream>

UnaryNode::UnaryNode(Token op, std::unique_ptr<ExprNode> right, size_t line)
    : op(op), right(std::move(right)), kind(ExprASTKind::Unary), line(line), type(SmplType::Unknown) { }

void UnaryNode::print() const {
    std::cout << " [" << op.lexeme << " ";
    right->print();
    std::cout << "]";
}
