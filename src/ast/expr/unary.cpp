#include "ast/expr/unary.hpp"
#include "error_reporter/compiler_err.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"
#include "token/tokenkind.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

UnaryNode::UnaryNode(Token op, std::unique_ptr<ExprNode> right)
    : op(op), right(std::move(right))
{
    type = get_type();
}

void UnaryNode::print() const {
    std::cout << magic_enum::enum_name(type) << " [" << op.lexeme << " ";
    right->print();
    std::cout << "]";
}

static inline bool is_numeric(SmplType type) {
    return (type >= SmplType::Int8 && type <= SmplType::Float128);
}

static inline bool is_integer(SmplType type) {
    return (type >= SmplType::Int8 && type <= SmplType::Uint);
}

static SmplType resolve_unary_type(TokenKind op, SmplType right_type) {
    switch (op) {
        case TokenKind::Minus:
            if (is_numeric(right_type)) return right_type;
            throw TypeError("Unary '-' operator requires operand to be numeric");
        case TokenKind::Not:
            if (right_type == SmplType::Boolean) return right_type;
            throw TypeError("'not' operator requires operand to be of type 'bool'");
        case TokenKind::Tilde:
            if (is_integer(right_type)) return right_type;
            throw TypeError("Bitwise 'not' operator requires operand to be an integer");
        default:
            throw SyntaxError("Unknown unary operator");
    }
}

SmplType UnaryNode::get_type() {
    SmplType right_type = right->get_type();

    return resolve_unary_type(op.kind, right_type);
}
