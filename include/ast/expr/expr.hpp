#pragma once

#include "smpl/types.hpp"

class ExprNode {
public:
    virtual ~ExprNode() = default;
    virtual void print() const = 0;
    virtual SmplType get_type() = 0;
};
