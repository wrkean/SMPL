#include "ast/expr/binary.hpp"
#include "error_reporter/compiler_err.hpp"
#include "smpl/types.hpp"
#include "token/tokenkind.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>

BinaryExpr::BinaryExpr(Token op, std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right)
    : op(op), left(std::move(left)), right(std::move(right))
{
    type = get_type();
}

void BinaryExpr::print() const {
    std::cout << "(" << magic_enum::enum_name(type) << " " << op.lexeme << " ";
    left->print();
    std::cout << " ";
    right->print();
    std::cout << ")";
}

static inline bool is_numeric(SmplType t) {
    return (t >= SmplType::Int8 && t <= SmplType::Float128);
}

static inline bool is_integer(SmplType t) {
    return (t >= SmplType::Int8 && t <= SmplType::Uint);
}

static SmplType promote(SmplType a, SmplType b) {
    if (a == SmplType::Unknown || b == SmplType::Unknown) return SmplType::Unknown;
    return ((static_cast<int>(a) > static_cast<int>(b)) ? a : b);
}

static SmplType resolve_binary_type(TokenKind op, SmplType left_type, SmplType right_type) {
    // Logical OR and AND
    if (op == TokenKind::And || op == TokenKind::Or) {
        if (left_type == SmplType::Boolean && right_type == SmplType::Boolean) {
            return SmplType::Boolean;
        }
        throw TypeError("Logical operators require both operands to be of type 'bool'");
    }

    // Relational operators
    if (op == TokenKind::EqualEqual
        || op == TokenKind::LesserEqual || op == TokenKind::LesserThan
        || op == TokenKind::GreaterEqual || op == TokenKind::GreaterThan) {
        if (left_type == right_type) return SmplType::Boolean;
        if (is_numeric(left_type) && is_numeric(right_type)) return SmplType::Boolean;
        throw TypeError("Invalid types for comparison");
    }

    if (op == TokenKind::As) return right_type; // Should be Unknown
    if (op == TokenKind::Range) {
        if (is_integer(left_type) && is_integer(right_type)) return SmplType::Unknown;
        throw TypeError("Operands in range '..' operator should be integers");
    }

    // Arithmetic operators
    if (is_numeric(left_type) && is_numeric(right_type)) {
        return promote(left_type, right_type);
    }

    throw TypeError("Invalid binary operation between types");
}

SmplType BinaryExpr::get_type() {
    SmplType left_type = left->get_type();
    SmplType right_type = right->get_type();

    return resolve_binary_type(op.kind, left_type, right_type);
}
