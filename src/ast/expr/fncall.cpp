#include "ast/expr/fncall.hpp"
#include <iostream>

FuncCallNode::FuncCallNode(Token identifier, std::vector<std::unique_ptr<ExprNode>> args)
    : identifier(identifier), args(std::move(args)) { }

void FuncCallNode::print() const {
    std::cout << "Identifier: " << identifier.lexeme << "\n";
    std::cout << "Args: ";
    for (auto& arg : args) {
        arg->print();
        std::cout << "\n";
    }
}
