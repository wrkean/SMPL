#include "ast/stmt/fndecl.hpp"
#include <iostream>

DefnNode::DefnNode(Token identifier, std::unique_ptr<ParamNode> params, Token return_type, std::unique_ptr<BlockNode> block)
    : identifier(identifier), params(std::move(params)), return_type(return_type), block(std::move(block)) { }

void DefnNode::print() const {
    std::cout << "Function name: " << identifier.lexeme << "\n";
    params->print();
    std::cout << "\n";
    std::cout << "Return type: " << return_type.lexeme << "\n";
    block->print();
    std::cout << "\n";
}
