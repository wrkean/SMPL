#pragma once

#include "ast/ast_kinds.hpp"

class ExprNode {
public:
    virtual ~ExprNode() = default;
    virtual void print() const = 0;
    virtual ExprASTKind get_kind() const = 0;
};
