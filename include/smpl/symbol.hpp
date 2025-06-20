#pragma once

#include "smpl/types.hpp"
#include "token/token.hpp"

class Symbol {
public:
    Symbol(SmplType type, Token symbol);

    SmplType type;
    Token symbol;
};
