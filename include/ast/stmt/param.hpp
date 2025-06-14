#pragma once

#include "ast/stmt/stmt.hpp"
#include "token/token.hpp"
#include <utility>
#include <vector>

class ParamNode : public Stmt {
public:
    ParamNode(std::vector<std::pair<Token, Token>> params);
    void print() const override;

    std::vector<std::pair<Token, Token>> params;
};
