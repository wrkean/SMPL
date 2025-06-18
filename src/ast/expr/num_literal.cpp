#include "ast/expr/num_literal.hpp"
#include "error_reporter/compiler_err.hpp"
#include "smpl/types.hpp"
#include "token/tokenkind.hpp"
#include <iostream>

NumLitNode::NumLitNode(Token literal)
    : literal(literal)
{
    type = get_type();
}

void NumLitNode::print() const {
    std::cout << literal.lexeme;
}

SmplType NumLitNode::get_type() const {
    switch (literal.kind) {
        case TokenKind::StringLiteral:
            return SmplType::String;

        case TokenKind::NumLiteral: {
            const std::string& lex = literal.lexeme;
            return lex.contains('.') ? SmplType::Float64 : SmplType::Int32;
        }

        default:
            throw SyntaxError("Invalid literal");
    }
}
