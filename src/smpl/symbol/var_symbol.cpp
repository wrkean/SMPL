#include "smpl/symbol/var_symbol.hpp"
#include "smpl/symbol/symbolkind.hpp"

VarSymbol::VarSymbol(SmplType type, const Token& token)
    : type(type), kind(SymbolKind::Variable), token(token) { }
