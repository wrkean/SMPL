#include "ast/stmt/param.hpp"
#include <iostream>
#include <utility>
#include <vector>

ParamNode::ParamNode(std::vector<std::pair<Token, Token>> params, size_t line)
    : params(params), kind(StmtASTKind::Param), line(line) { }

void ParamNode::print() const {
    for (auto& param : params) {
        std::cout << param.first.lexeme;
        std::cout << " ";
        std::cout << param.second.lexeme;
        std::cout << " ";
    }
}
