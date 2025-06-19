#include "ast/expr/num_literal.hpp"
#include "error_reporter/compiler_err.hpp"
#include "smpl/types.hpp"
#include "token/tokenkind.hpp"
#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <utility>
#include <vector>

NumLitNode::NumLitNode(Token literal)
    : literal(literal), kind(ExprASTKind::NumberLiteral)

{
    type = get_type();
}

void NumLitNode::print() const {
    std::cout << magic_enum::enum_name(type) << " " << literal.lexeme;
}

SmplType NumLitNode::get_type() {
    std::string& lex = literal.lexeme;
    const std::vector<std::pair<std::string, SmplType>> suffix_map = {
        {"f128", SmplType::Float128},
        {"f64", SmplType::Float64},
        {"f32", SmplType::Float32},
        {"f16", SmplType::Float16},
        {"i64", SmplType::Int64},
        {"u64", SmplType::Uint64},
        {"i32", SmplType::Int32},
        {"u32", SmplType::Uint32},
        {"i16", SmplType::Int16},
        {"u16", SmplType::Uint16},
        {"i8",  SmplType::Int8},
        {"u8",  SmplType::Uint8},
    };

    for (const auto& [sfx, type] : suffix_map) {
        SmplType t;
        if (lex.ends_with(sfx)) {
            t = type;
        }
        lex = lex.substr(0, lex.length() - sfx.length());
    }

    switch (literal.kind) {
        case TokenKind::NumLiteral: {
            return lex.contains('.') ? SmplType::Float64 : SmplType::Int32;
        }
        default:
            throw SyntaxError("Invalid literal");
    }
}
