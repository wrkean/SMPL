#include "ast/expr/fncall.hpp"
#include "smpl/types.hpp"
#include <iostream>

FuncCallNode::FuncCallNode(Token identifier, std::vector<std::unique_ptr<ExprNode>> args)
    : identifier(identifier), args(std::move(args)), kind(ExprASTKind::FnCall)

{
    type = get_type();
}

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

SmplType FuncCallNode::get_type() {
    // Can't deduce yet
    return SmplType::Unknown;
}
