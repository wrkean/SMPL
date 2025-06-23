
#include "ast/expr/type.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

TypeNode::TypeNode(Token type, size_t line)
    : type(type), kind(ExprASTKind::Type), line(line) { }

void TypeNode::print() const {
    std::cout << type.lexeme;
}
