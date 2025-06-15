#pragma once

#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include "token/token.hpp"
#include "token/tokenkind.hpp"
#include <memory>
#include <vector>

enum class Assoc { Left, Right };

struct OperatorInfo {
    int precedence;
    Assoc associativity;
};

class Parser {
public:
    // Parser will own the output from the Lexer (vector of tokens)
    Parser(const std::vector<Token>&& tokens);
    std::vector<std::unique_ptr<StmtNode>> parse();

private:
    std::vector<Token> tokens;
    size_t current;

    // Private methods
    //
    // Statements
    std::unique_ptr<StmtNode> parse_statement();
    std::unique_ptr<StmtNode> parse_fndecl();
    std::unique_ptr<StmtNode> parse_params();
    std::unique_ptr<StmtNode> parse_block();
    std::unique_ptr<StmtNode> parse_assignment();
    std::unique_ptr<StmtNode> parse_return();
    std::unique_ptr<StmtNode> parse_for();

    // Expressions
    std::unique_ptr<ExprNode> parse_expression(int prec = 0);
    std::unique_ptr<ExprNode> parse_fncall(Token id);
    std::unique_ptr<ExprNode> nud(Token token);
    std::unique_ptr<ExprNode> led(Token op, std::unique_ptr<ExprNode> left);

    // Helper methods
    int get_precedence(Token op);
    Assoc get_associativity(Token op);
    Token consume(TokenKind expected);
    Token advance();
    Token peek() const;
    bool match(TokenKind kind);
    bool at_end() const;
};
