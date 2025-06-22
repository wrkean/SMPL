#pragma once

#include "ast/ast_kinds.hpp"
#include "ast/stmt/stmt.hpp"
#include "token/token.hpp"
#include <utility>
#include <vector>

class ParamNode : public StmtNode {
public:
    ParamNode(std::vector<std::pair<Token, Token>> params, size_t line);
    void print() const override;
    StmtASTKind get_kind() const override { return kind; }
    size_t get_line() const override { return line; }

    std::vector<std::pair<Token, Token>> params;
    StmtASTKind kind;
    size_t line;
};
