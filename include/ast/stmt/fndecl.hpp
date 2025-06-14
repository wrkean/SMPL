#pragma once

#include "ast/stmt/block.hpp"
#include "ast/stmt/param.hpp"
#include "ast/stmt/stmt.hpp"
#include "token/token.hpp"
#include <memory>

class DefnNode : public Stmt {
public:
    DefnNode(Token identifier, std::unique_ptr<ParamNode> params, Token return_type, std::unique_ptr<BlockNode> block);
    void print() const override;

    Token identifier;
    std::unique_ptr<ParamNode> params;
    Token return_type;
    std::unique_ptr<BlockNode> block;
};
