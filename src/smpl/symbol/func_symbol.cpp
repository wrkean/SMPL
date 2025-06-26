#include "smpl/symbol/func_symbol.hpp"
#include "smpl/symbol/symbolkind.hpp"

FuncSymbol::FuncSymbol(SmplType return_type, std::vector<SmplType> param_types, const Token& token)
    : return_type(return_type), kind(SymbolKind::Function), token(token), param_types(param_types) { }
