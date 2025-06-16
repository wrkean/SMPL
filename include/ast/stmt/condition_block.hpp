#pragma once

#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include <memory>

struct ConditionBlock {
    ConditionBlock(std::unique_ptr<ExprNode> condition, std::unique_ptr<StmtNode> block);

    std::unique_ptr<ExprNode> condition;
    std::unique_ptr<StmtNode> block;
};
