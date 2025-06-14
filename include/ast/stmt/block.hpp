#pragma once

#include "ast/stmt/stmt.hpp"
#include <memory>
#include <vector>

class BlockNode : public Stmt {
public:
    BlockNode(std::vector<std::unique_ptr<Stmt>> statements);
    void print() const override;

    std::vector<std::unique_ptr<Stmt>> statements;
};
