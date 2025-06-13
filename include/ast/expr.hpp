#pragma once

#include "ast/visitor/visitor.hpp"

class ExprNode {
public:
    virtual ~ExprNode() = default;
    virtual void accept(Visitor& visitor) = 0;
};
