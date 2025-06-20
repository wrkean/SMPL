#pragma once

#include "ast/ast_kinds.hpp"
class StmtNode {
public:
    virtual ~StmtNode() = default;
    virtual void print() const = 0;
    virtual StmtASTKind get_kind() const = 0;
};
