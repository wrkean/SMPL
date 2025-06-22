#include "ast/expr/binary.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

BinaryExpr::BinaryExpr(Token op, std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right, size_t line)
    : op(op), left(std::move(left)), right(std::move(right)), kind(ExprASTKind::Binary), line(line) { }

void BinaryExpr::print() const {
    std::cout << "(" << " " << op.lexeme << " ";
    left->print();
    std::cout << " ";
    right->print();
    std::cout << ")";
}
