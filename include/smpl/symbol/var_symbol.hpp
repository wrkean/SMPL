#pragma once


#include "smpl/symbol/symbol.hpp"
#include "smpl/symbol/symbolkind.hpp"
#include "smpl/types.hpp"
#include "token/token.hpp"

class VarSymbol : public Symbol {
public:
    VarSymbol(SmplType type, const Token& token);
    SymbolKind get_kind() const override { return kind; }
    SmplType get_type() const override { return type; }
    Token get_token() const override { return token; }

    SymbolKind kind;
    SmplType type;
    Token token;
};
