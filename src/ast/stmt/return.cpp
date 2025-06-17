#include "ast/stmt/return.hpp"
#include <iostream>
#include <memory>

ReturnNode::ReturnNode(std::unique_ptr<ExprNode> return_expr)
    : return_expr(std::move(return_expr)) { }

void ReturnNode::print() const {
    std::cout << "Return: ";
    return_expr->print();
}
