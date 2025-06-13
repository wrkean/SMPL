#pragma once

#include "ast/expr.hpp"
#include "ast/grouping.hpp"
#include "visitor/visitor.hpp"

class ASTPrinter : public Visitor {
public:
    void print(ExprNode& expr);
    void visit(BinaryExpr& expr) override;
    void visit(NumLitNode& expr) override;
    void visit(GroupingExpr& expr) override;
};
