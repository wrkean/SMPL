#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/stmt/stmt.hpp"
#include "token/token.hpp"
#include <memory>
#include <optional>

class DefnNode : public StmtNode {
public:
    DefnNode(Token identifier, std::unique_ptr<StmtNode> params, std::optional<Token> return_type, std::unique_ptr<StmtNode> block);
    void print() const override;
    StmtASTKind get_kind() const override { return kind; }

    Token identifier;
    std::unique_ptr<StmtNode> params;
    std::optional<Token> return_type;
    std::unique_ptr<StmtNode> block;
    StmtASTKind kind;
};
