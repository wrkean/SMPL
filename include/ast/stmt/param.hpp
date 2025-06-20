#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/stmt/stmt.hpp"
#include "token/token.hpp"
#include <utility>
#include <vector>

class ParamNode : public StmtNode {
public:
    ParamNode(std::vector<std::pair<Token, Token>> params);
    void print() const override;
    StmtASTKind get_kind() const override { return kind; }

    std::vector<std::pair<Token, Token>> params;
    StmtASTKind kind;
};
