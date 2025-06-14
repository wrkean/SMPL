#pragma once

#include "ast/expr/expr.hpp"
#include "token/token.hpp"
#include <memory>
#include <vector>

class Parser {
public:
    // Parser will own the output from the Lexer (vector of tokens)
    Parser(const std::vector<Token>&& tokens);
    std::unique_ptr<ExprNode> parse();

private:
    std::vector<Token> tokens;
    size_t current;

    // Private methods
    std::unique_ptr<ExprNode> parse_expression(int prec = 0);
    std::unique_ptr<ExprNode> nud(Token token);
    std::unique_ptr<ExprNode> led(Token op, std::unique_ptr<ExprNode> left);

    // Helper methods
    int get_precedence(Token op);
    Token advance();
    Token peek() const;
    bool at_end() const;
};
