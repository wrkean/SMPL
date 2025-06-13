#pragma once

class BinaryExpr;
class NumLitNode;
class GroupingExpr;

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(BinaryExpr& expr) = 0;
    virtual void visit(NumLitNode& expr) = 0;
    virtual void visit(GroupingExpr& expr) = 0;
};
