#pragma once

#include "smpl/symbol/symbol.hpp"
#include "smpl/symbol/symbolkind.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"
#include <vector>
class FuncSymbol : public Symbol {
public:
    FuncSymbol(SmplType return_type, std::vector<SmplType> param_types, const Token& token);

    SymbolKind get_kind() const override { return kind; }
    SmplType get_type() const override { return return_type; }
    Token get_token() const override { return token; }

    SymbolKind kind;
    SmplType return_type;
    Token token;
    std::vector<SmplType> param_types;
};
