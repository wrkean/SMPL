#include "ast/visitor/ast_printer.hpp"
#include "ast/binary.hpp"
#include "ast/expr.hpp"
#include "ast/num_literal.hpp"
#include <iostream>

void ASTPrinter::print(ExprNode& expr) {
    expr.accept(*this);
}

void ASTPrinter::visit(BinaryExpr& expr) {
    std::cout << "(" << expr.op.lexeme << " ";
    expr.left->accept(*this);
    std::cout << " ";
    expr.right->accept(*this);
    std::cout << ")";
}

void ASTPrinter::visit(NumLitNode& expr) {
    std::cout << expr.literal.lexeme;
}
