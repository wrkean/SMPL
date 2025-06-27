#pragma once

#include "ast/ast_kinds.hpp"
#include "smpl/types.hpp"
#include <cstddef>

class ExprNode {
public:
    virtual ~ExprNode() = default;
    virtual void print() const = 0;
    virtual ExprASTKind get_kind() const = 0;
    virtual SmplType get_type() const = 0;
    virtual size_t get_line() const = 0;

    SmplType type = SmplType::Unknown;
};
