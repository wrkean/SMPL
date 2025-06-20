#include "smpl/symbol.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"

Symbol::Symbol(SmplType type, Token symbol)
    : type(type), symbol(symbol) { }
