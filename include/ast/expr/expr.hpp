#pragma once

class ExprNode {
public:
    virtual ~ExprNode() = default;
    virtual void print() const = 0;
};
