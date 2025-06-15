#include "parser/parser.hpp"
#include "ast/expr/binary.hpp"
#include "ast/expr/expr.hpp"
#include "ast/expr/fncall.hpp"
#include "ast/expr/grouping.hpp"
#include "ast/expr/identifier.hpp"
#include "ast/expr/num_literal.hpp"
#include "ast/stmt/assignment.hpp"
#include "ast/stmt/block.hpp"
#include "ast/stmt/fndecl.hpp"
#include "ast/stmt/for.hpp"
#include "ast/stmt/param.hpp"
#include "ast/stmt/return.hpp"
#include "ast/stmt/stmt.hpp"
#include "smpl.hpp"
#include "token/token.hpp"
#include "token/tokenkind.hpp"
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

static std::unordered_map<TokenKind, OperatorInfo> operator_table = {
    {TokenKind::Equal,     {1, Assoc::Right}}, // assignment
    // {TokenKind::Or,        {2, Assoc::Left}},
    // {TokenKind::And,       {3, Assoc::Left}},
    {TokenKind::Range,     {5, Assoc::Left}},
    {TokenKind::Plus,      {10, Assoc::Left}},
    {TokenKind::Minus,     {10, Assoc::Left}},
    {TokenKind::Star,      {20, Assoc::Left}},
    {TokenKind::ForSlash,  {20, Assoc::Left}},
    // {TokenKind::Power,     {30, Assoc::Right}}, // e.g. **
};

Parser::Parser(const std::vector<Token>&& tokens)
    : tokens(std::move(tokens)), current(0) { }

std::vector<std::unique_ptr<StmtNode>> Parser::parse() {
    std::vector<std::unique_ptr<StmtNode>> statements;

    while (!at_end()) {
        statements.push_back(std::move(parse_statement()));
    }

    return statements;
}

std::unique_ptr<StmtNode> Parser::parse_statement() {
    std::cout << "Current token: " << peek().lexeme << "\n";
    switch (peek().kind) {
        case TokenKind::Defn:
            return parse_fndecl();
        case TokenKind::Let:
            return parse_assignment();
        case TokenKind::Return:
            return parse_return();
        case TokenKind::For:
            return parse_for();
        // TODO: Parse more statements
        default:
            Smpl::error(peek().line, std::format("Unexpected token: {}", peek().lexeme));
    }
}

std::unique_ptr<StmtNode> Parser::parse_fndecl() {
    consume(TokenKind::Defn);
    Token identifier = consume(TokenKind::Identifier);

    consume(TokenKind::LeftParen);
    auto params = parse_params();
    consume(TokenKind::RightParen);

    // Get return type
    std::optional<Token> return_type;
    if (peek().kind == TokenKind::ThinArrow) {
        advance();      // Consume '->'
        return_type = consume(TokenKind::Identifier);
    } else if (peek().kind != TokenKind::LeftBrace) {
        Smpl::error(peek().line, "Expected '{' or '->'");
    }

    consume(TokenKind::LeftBrace);
    auto block = parse_block();
    consume(TokenKind::RightBrace);

    return std::make_unique<DefnNode>(DefnNode(identifier, std::move(params), return_type, std::move(block)));
}

std::unique_ptr<StmtNode> Parser::parse_block() {
    std::vector<std::unique_ptr<StmtNode>> statements;

    while (peek().kind != TokenKind::RightBrace) {
        statements.push_back(std::move(parse_statement()));
    }

    return std::make_unique<BlockNode>(BlockNode(std::move(statements)));
}

std::unique_ptr<StmtNode> Parser::parse_params() {
    std::vector<std::pair<Token, Token>> params;
    while (peek().kind != TokenKind::RightParen) {
        if (peek().kind == TokenKind::Identifier) {
            Token param_id = advance();
            consume(TokenKind::Colon);
            Token param_type = consume(TokenKind::Identifier);
            params.emplace_back(param_id, param_type);
            if (peek().kind == TokenKind::Comma) {
                advance();
            } else if (peek().kind != TokenKind::RightParen) {
                // FIXME: Handle errors gracefully
                throw std::runtime_error("Expected ')' or ','");
            }
        }
    }

    return std::make_unique<ParamNode>(ParamNode(params));
}

std::unique_ptr<StmtNode> Parser::parse_assignment() {
    consume(TokenKind::Let);
    Token identifier = consume(TokenKind::Identifier);

    consume(TokenKind::Colon);
    Token type = consume(TokenKind::Identifier);

    // TODO: Only handles equal operators! add more assignment operators like +=.
    Token op = consume(TokenKind::Equal);
    auto right = parse_expression();
    consume(TokenKind::SemiColon);

    return std::make_unique<AssignmentNode>(AssignmentNode(identifier, type, op, std::move(right)));
}

std::unique_ptr<StmtNode> Parser::parse_return() {
    consume(TokenKind::Return);
    auto expr = parse_expression();
    consume(TokenKind::SemiColon);
    return std::make_unique<ReturnNode>(ReturnNode(std::move(expr)));
}

std::unique_ptr<StmtNode> Parser::parse_for() {
    consume(TokenKind::For);
    Token bind_var = consume(TokenKind::Identifier);
    consume(TokenKind::In);
    auto iterator = parse_expression();
    consume(TokenKind::LeftBrace);
    auto block = parse_block();
    consume(TokenKind::RightBrace);

    return std::make_unique<ForNode>(ForNode(bind_var, std::move(iterator), std::move(block)));
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

std::unique_ptr<ExprNode> Parser::parse_fncall(Token id) {
    std::vector<std::unique_ptr<ExprNode>> args;
    consume(TokenKind::LeftParen);

    while (peek().kind != TokenKind::RightParen) {
        args.push_back(std::move(parse_expression()));
        if (peek().kind == TokenKind::Comma) {
            advance();
        } else if (peek().kind != TokenKind::RightParen) {
            // FIXME: Handle errors gracefully
            throw std::runtime_error("Expected ')' or ','");
        }
    }
    consume(TokenKind::RightParen);
    return std::make_unique<FuncCallNode>(FuncCallNode(id, std::move(args)));
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
        case TokenKind::Identifier: {
            Token id = advance();
            if (peek().kind == TokenKind::LeftParen) {
                auto fncall = parse_fncall(id);
                return fncall;
            }
            return std::make_unique<IdentifierNode>(IdentifierNode(id));
        }
        default:
            Smpl::error(peek().line, std::format("Current token: {}", peek().lexeme));
            // TODO: Error just for debugging, change later
            throw std::runtime_error("Expected expression");
    }
}

std::unique_ptr<ExprNode> Parser::led(Token op, std::unique_ptr<ExprNode> left) {
    int prec = get_precedence(op);
    Assoc assoc = get_associativity(op);
    int next_prec = (assoc == Assoc::Left) ? prec : prec - 1;

    auto right = parse_expression(next_prec);
    return std::make_unique<BinaryExpr>(BinaryExpr(op, std::move(left), std::move(right)));
}

int Parser::get_precedence(Token op) {
    if (operator_table.contains(op.kind)) {
        auto op_info = operator_table.at(op.kind);
        return op_info.precedence;
    }
    return 0;
}

Assoc Parser::get_associativity(Token op) {
    auto op_info = operator_table.at(op.kind);
    return op_info.associativity;
}

Token Parser::consume(TokenKind expected) {
    if (expected == peek().kind) return advance();

    Smpl::error(peek().line, std::format("Unexpected token: {}", peek().lexeme));
    throw std::runtime_error("Unexpected token");
}

Token Parser::advance() {
    current++;
    return tokens[current - 1];
}

Token Parser::peek() const {
    return tokens[current];
}

bool Parser::match(TokenKind kind) {
    if (peek().kind == kind) {
        advance();
        return true;
    }
    return false;
}

bool Parser::at_end() const {
    return peek().kind == TokenKind::Eof;
}
