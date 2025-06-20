#include "ast/expr/fncall.hpp"
#include <iostream>

FuncCallNode::FuncCallNode(Token identifier, std::vector<std::unique_ptr<ExprNode>> args)
    : identifier(identifier), args(std::move(args)), kind(ExprASTKind::FnCall) { }

void FuncCallNode::print() const {
    std::cout << "Function call\n";
    std::cout << "Identifier: " << identifier.lexeme << "\n";
    std::cout << "Args: ";
    for (auto& arg : args) {
        arg->print();
        std::cout << " ";
    }
    std::cout << "\nEnd function call";
}
