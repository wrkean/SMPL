#pragma once

#include "smpl/symbol/symbolkind.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"
class Symbol {
public:
    virtual ~Symbol() = default;
    virtual SymbolKind get_kind() const = 0;
    virtual SmplType get_type() const = 0;
    virtual Token get_token() const = 0;
};



// #include "smpl/types.hpp"
// #include "token/token.hpp"
// #include <vector>
//
// class Symbol {
// public:
//     Symbol(SmplType type, const Token& token);
//     Symbol(SmplType return_type, const std::vector<SmplType>& param_types, const Token& token);
//
//     SmplType type;
//     std::vector<SmplType> param_types;
//     Token token;
//     bool is_function;
// };
