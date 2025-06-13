#include "parser/parser.hpp"
#include "ast/binary.hpp"
#include "ast/expr.hpp"
#include "ast/grouping.hpp"
#include "ast/num_literal.hpp"
#include "smpl.hpp"
#include "token/tokenkind.hpp"
#include <memory>
#include <stdexcept>
#include <vector>

Parser::Parser(const std::vector<Token>&& tokens)
    : tokens(std::move(tokens)), current(0) { }

std::unique_ptr<ExprNode> Parser::parse() {
    return parse_expression(0);
}

std::unique_ptr<ExprNode> Parser::parse_expression(int prec) {
    auto left = nud(peek());

    while (!at_end() && get_precedence(peek()) > prec) {
        Token op = peek();
        advance();
        left = led(op, std::move(left));
    }

    return left;
}

std::unique_ptr<ExprNode> Parser::nud(Token token) {
    switch (token.kind) {
        case TokenKind::NumLiteral:
            advance();
            return std::make_unique<NumLitNode>(NumLitNode(token));
        case TokenKind::LeftParen: {
            advance();  // Consume '('
            auto expr = parse_expression();
            if (peek().kind != TokenKind::RightParen) {
                Smpl::error(peek().line, "Expected ')'");
            }
            advance();  // Consume ')'
            return std::make_unique<GroupingExpr>(GroupingExpr(std::move(expr)));
        }
        default:
            // TODO: Error just for debugging, change later
            throw std::runtime_error("Expected expression");
    }
}

std::unique_ptr<ExprNode> Parser::led(Token op, std::unique_ptr<ExprNode> left) {
    int prec = get_precedence(op);
    auto right = parse_expression(prec);
    return std::make_unique<BinaryExpr>(BinaryExpr(op, std::move(left), std::move(right)));
}

int Parser::get_precedence(Token op) {
    switch (op.kind) {
        case TokenKind::Plus:
        case TokenKind::Minus:
            return 10;
        case TokenKind::Star:
        case TokenKind::ForSlash:
            return 20;
        default:
            return 0;
    }
}

Token Parser::advance() {
    current++;
    return tokens[current - 1];
}

Token Parser::peek() const {
    return tokens[current];
}

bool Parser::at_end() const {
    return current >= tokens.size();
}
