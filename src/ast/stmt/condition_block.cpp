#include "ast/stmt/condition_block.hpp"

ConditionBlock::ConditionBlock(std::unique_ptr<ExprNode> condition, std::unique_ptr<StmtNode> block)
    : condition(std::move(condition)), block(std::move(block)) { }
