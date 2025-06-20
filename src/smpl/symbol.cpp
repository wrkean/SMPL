#include "smpl/symbol.hpp"
#include "token/token.hpp"

Symbol::Symbol(SmplType type, const Token& token)
    : type(type), token(token), is_function(false) { }
Symbol::Symbol(SmplType return_type, const std::vector<SmplType>& param_types, const Token& token)
    : type(return_type), param_types(param_types), token(token), is_function(true) { }
