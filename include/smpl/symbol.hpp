#pragma once

#include "smpl/types.hpp"
#include "token/token.hpp"
#include <vector>

class Symbol {
public:
    Symbol(SmplType type, const Token& token);
    Symbol(SmplType return_type, const std::vector<SmplType>& param_types, const Token& token);

    SmplType type;
    std::vector<SmplType> param_types;
    Token token;
    bool is_function;
};
