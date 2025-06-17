#include "ast/stmt/fndecl.hpp"
#include <iostream>

DefnNode::DefnNode(Token identifier, std::unique_ptr<StmtNode> params, std::optional<Token> return_type, std::unique_ptr<StmtNode> block)
    : identifier(identifier), params(std::move(params)), return_type(return_type), block(std::move(block)) { }

void DefnNode::print() const {
    std::cout << "Function name: " << identifier.lexeme << "\n";
    std::cout << "Params: ";
    params->print();
    std::cout << "\n";
    std::cout << "Return type: ";
    std::cout << (return_type.has_value() ? return_type.value().lexeme : "") << "\n";
    block->print();
    std::cout << "End " << identifier.lexeme << "\n";
}
