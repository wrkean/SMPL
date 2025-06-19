#include "parser/parser.hpp"
#include "ast/expr/binary.hpp"
#include "ast/expr/boolean_literal.hpp"
#include "ast/expr/expr.hpp"
#include "ast/expr/fncall.hpp"
#include "ast/expr/grouping.hpp"
#include "ast/expr/identifier.hpp"
#include "ast/expr/num_literal.hpp"
#include "ast/expr/unary.hpp"
#include "ast/stmt/assignment.hpp"
#include "ast/stmt/block.hpp"
#include "ast/stmt/condition_block.hpp"
#include "ast/stmt/expr_stmt.hpp"
#include "ast/stmt/fndecl.hpp"
#include "ast/stmt/for.hpp"
#include "ast/stmt/if.hpp"
#include "ast/stmt/param.hpp"
#include "ast/stmt/return.hpp"
#include "ast/stmt/stmt.hpp"
#include "ast/stmt/while.hpp"
#include "error_reporter/compiler_err.hpp"
#include "smpl/smpl.hpp"
#include "token/token.hpp"
#include "token/tokenkind.hpp"
#include <format>
#include <initializer_list>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <optional>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

// For infix operators
static std::unordered_map<TokenKind, OperatorInfo> operator_table = {
    // Precedence 1: Assignment (right-to-left)
    {TokenKind::Equal,                  {1, Assoc::Right}},
    {TokenKind::PlusEqual,              {1, Assoc::Right}},
    {TokenKind::MinusEqual,             {1, Assoc::Right}},
    {TokenKind::StarEqual,              {1, Assoc::Right}},
    {TokenKind::ForSlashEqual,          {1, Assoc::Right}},
    {TokenKind::PercentEqual,           {1, Assoc::Right}},
    {TokenKind::AmperEqual,             {1, Assoc::Right}},
    {TokenKind::CaretEqual,             {1, Assoc::Right}},
    {TokenKind::PipeEqual,              {1, Assoc::Right}},
    {TokenKind::LesserLesserEqual,      {1, Assoc::Right}},
    {TokenKind::GreaterGreaterEqual,   {1, Assoc::Right}},

    // Precedence 2: Logical OR
    {TokenKind::Or,                    {2, Assoc::Left}},

    // Precedence 3: Logical AND
    {TokenKind::And,                   {3, Assoc::Left}},

    // Precedence 4: Bitwise OR
    {TokenKind::Pipe,                  {4, Assoc::Left}},

    // Precedence 5: Bitwise XOR
    {TokenKind::Caret,                 {5, Assoc::Left}},

    // Precedence 6: Bitwise AND
    {TokenKind::Ampersand,             {6, Assoc::Left}},

    // Precedence 7: Equality
    {TokenKind::EqualEqual,            {7, Assoc::Left}},
    {TokenKind::NotEqual,              {7, Assoc::Left}},

    // Precedence 8: Relational
    {TokenKind::LesserThan,            {8, Assoc::Left}},
    {TokenKind::LesserEqual,           {8, Assoc::Left}},
    {TokenKind::GreaterThan,           {8, Assoc::Left}},
    {TokenKind::GreaterEqual,          {8, Assoc::Left}},

    // Precedence 9: Bitshift, Range '..' operator, and the type cast 'as' operator
    {TokenKind::LesserLesser,          {9, Assoc::Left}},
    {TokenKind::GreaterGreater,        {9, Assoc::Left}},
    {TokenKind::Range,                 {9, Assoc::Left}},
    {TokenKind::As,                    {9, Assoc::Left}},

    // Precedence 10: Additive
    {TokenKind::Plus,                  {10, Assoc::Left}},
    {TokenKind::Minus,                 {10, Assoc::Left}},

    // Precedence 11: Multiplicative
    {TokenKind::Star,                  {11, Assoc::Left}},
    {TokenKind::ForSlash,              {11, Assoc::Left}},
    {TokenKind::Percent,               {11, Assoc::Left}},
};

Parser::Parser(const std::vector<Token>&& tokens)
    : tokens(std::move(tokens)), current(0) { }

std::vector<std::unique_ptr<StmtNode>> Parser::parse() {
    std::vector<std::unique_ptr<StmtNode>> statements;

    while (!at_end()) {
        try {
            auto stmt = parse_statement();
            if (stmt) statements.push_back(std::move(stmt));
        } catch (const CompilerError& err) {
            Smpl::error(peek().line, err.what());
            synchronize();
        }
    }

    return statements;
}

std::unique_ptr<StmtNode> Parser::parse_statement() {
    switch (peek().kind) {
        // TODO: Parse more statements
        case TokenKind::Defn:
            return parse_fndecl();
        case TokenKind::Let:
            return parse_assignment();
        case TokenKind::Return:
            return parse_return();
        case TokenKind::For:
            return parse_for();
        case TokenKind::If:
            return parse_if();
        case TokenKind::While:
            return parse_while();
        case TokenKind::Identifier:
        case TokenKind::NumLiteral:
        case TokenKind::True:
        case TokenKind::False:
        case TokenKind::LeftParen:
        case TokenKind::Not:
        case TokenKind::Minus:
            return parse_expr_stmt();
        default:
            throw SyntaxError(std::format("Unexpected token: {}", peek().lexeme));
    }
}

std::unique_ptr<StmtNode> Parser::parse_fndecl() {
    consume(TokenKind::Defn);
    Token identifier = consume(TokenKind::Identifier);

    auto params = parse_params();

    // Get return type
    std::optional<Token> return_type;
    if (peek().kind == TokenKind::ThinArrow) {
        advance();      // Consume '->'
        return_type = consume(TokenKind::Identifier);
    } else if (peek().kind != TokenKind::LeftBrace) {
        throw SyntaxError("Expected block{} or '->'");
    }

    auto block = parse_block();
    return std::make_unique<DefnNode>(identifier, std::move(params), return_type, std::move(block));
}

std::unique_ptr<StmtNode> Parser::parse_block() {
    consume(TokenKind::LeftBrace);
    std::vector<std::unique_ptr<StmtNode>> statements;

    while (peek().kind != TokenKind::RightBrace) {
        try {
            auto stmt = parse_statement();
            if (stmt) statements.push_back(std::move(stmt));
        } catch (const CompilerError& err) {
            Smpl::error(peek().line, err.what());
            synchronize();
        }
    }
    consume(TokenKind::RightBrace);
    return std::make_unique<BlockNode>(std::move(statements));
}

std::unique_ptr<StmtNode> Parser::parse_params() {
    consume(TokenKind::LeftParen);
    std::vector<std::pair<Token, Token>> params;

    while (peek().kind != TokenKind::RightParen) {
        Token param_id = consume(TokenKind::Identifier);
        consume(TokenKind::Colon);

        Token param_type = consume(TokenKind::Identifier);
        params.emplace_back(param_id, param_type);

        if (peek().kind == TokenKind::Comma) {
            advance();
        } else if (peek().kind != TokenKind::RightParen) {
            throw SyntaxError("Expected ')' or ','");
        }
    }
    consume(TokenKind::RightParen);
    return std::make_unique<ParamNode>(params);
}

std::unique_ptr<StmtNode> Parser::parse_assignment() {
    consume(TokenKind::Let);
    Token identifier = consume(TokenKind::Identifier);

    consume(TokenKind::Colon);
    Token type = consume(TokenKind::Identifier);

    consume(TokenKind::Equal);
    auto right = parse_expression();
    consume(TokenKind::SemiColon);

    return std::make_unique<AssignmentNode>(identifier, type, std::move(right));
}

std::unique_ptr<StmtNode> Parser::parse_return() {
    consume(TokenKind::Return);
    auto expr = parse_expression();
    consume(TokenKind::SemiColon);
    return std::make_unique<ReturnNode>(std::move(expr));
}

std::unique_ptr<StmtNode> Parser::parse_for() {
    consume(TokenKind::For);
    Token bind_var = consume(TokenKind::Identifier);
    consume(TokenKind::In);
    auto iterator = parse_expression();
    auto block = parse_block();

    return std::make_unique<ForNode>(bind_var, std::move(iterator), std::move(block));
}

std::unique_ptr<StmtNode> Parser::parse_if() {
    std::vector<ConditionBlock> branches;

    consume(TokenKind::If);

    auto cond = parse_expression();
    auto block = parse_block();
    branches.emplace_back(std::move(cond), std::move(block));

    while (match(TokenKind::Else)) {
        if (peek().kind == TokenKind::If) {
            advance();  // Consume 'if'
            auto branch_cond = parse_expression();
            auto branch_block = parse_block();
            branches.emplace_back(std::move(branch_cond), std::move(branch_block));
            continue;
        }
        std::unique_ptr<ExprNode> branch_cond = nullptr;    // Indicates this is an else block
        auto branch_block = parse_block();
        branches.emplace_back(std::move(branch_cond), std::move(branch_block));
        break;
    }

    return std::make_unique<IfNode>(std::move(branches));
}

std::unique_ptr<StmtNode> Parser::parse_while() {
    consume(TokenKind::While);
    auto cond = parse_expression();
    auto block = parse_block();

    return std::make_unique<WhileNode>(std::move(cond), std::move(block));
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
            throw SyntaxError("Expexted ')' or ','");
        }
    }
    consume(TokenKind::RightParen);
    return std::make_unique<FuncCallNode>(id, std::move(args));
}

std::unique_ptr<StmtNode> Parser::parse_expr_stmt() {
    auto expr = parse_expression();
    consume(TokenKind::SemiColon);
    return std::make_unique<ExprStmt>(std::move(expr));
}

std::unique_ptr<ExprNode> Parser::nud(Token token) {
    switch (token.kind) {
        case TokenKind::NumLiteral:
            advance();
            return std::make_unique<NumLitNode>(NumLitNode(token));
        case TokenKind::LeftParen: {
            advance();  // Consume '('
            auto expr = parse_expression();
            consume(TokenKind::RightParen);
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
        case TokenKind::True:
        case TokenKind::False:
            advance();
            return std::make_unique<BooleanLiteral>(token);
        case TokenKind::Not:
        case TokenKind::Minus: {
            Token op = advance();
            int unary_prec = 100;
            auto right = parse_expression(100);
            return std::make_unique<UnaryNode>(op, std::move(right));
        }
        default:
            throw SyntaxError("Expected an expression");
    }
}

std::unique_ptr<ExprNode> Parser::led(Token op, std::unique_ptr<ExprNode> left) {
    int prec = get_precedence(op);
    Assoc assoc = get_associativity(op);
    int next_prec = (assoc == Assoc::Left) ? prec : prec - 1;

    auto right = parse_expression(next_prec);
    return std::make_unique<BinaryExpr>(BinaryExpr(op, std::move(left), std::move(right)));
}

void Parser::synchronize() {
    advance();  // Consume erroneous token

    while (!at_end()) {
        if (previous().kind == TokenKind::SemiColon) return;

        switch (peek().kind) {
            case TokenKind::Defn:
            case TokenKind::Let:
            case TokenKind::Return:
            case TokenKind::If:
            case TokenKind::While:
                return;
            default:    // Only cancels warning (Unhandled enum values)
        }

        advance();
    }
}

int Parser::get_precedence(Token op) {
    if (operator_table.contains(op.kind)) {
        auto op_info = operator_table.at(op.kind);
        return op_info.precedence;
    }
    return 0;
}

Assoc Parser::get_associativity(Token op) {
    if (operator_table.contains(op.kind)) {
        auto op_info = operator_table.at(op.kind);
        return op_info.associativity;
    }

    throw SyntaxError(std::format("Unexpected operator '{}' in expression", op.lexeme));
}

Token Parser::consume(TokenKind expected) {
    if (expected == peek().kind) return advance();

    std::ostringstream oss;
    std::ostringstream oss2;
    oss << magic_enum::enum_name(peek().kind);
    oss2 << magic_enum::enum_name(expected);

    throw SyntaxError(std::format("Unexpected token <{}>, expected <{}>.", oss.str(), oss2.str()));
}

// TODO: should I remove this?
Token Parser::consume_any(std::initializer_list<TokenKind> expected_kinds, const std::string& err_msg) {
    for (auto& kind : expected_kinds) {
        if (kind == peek().kind) return advance();
    }

    throw SyntaxError(err_msg);
}

Token Parser::advance() {
    current++;
    return tokens[current - 1];
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
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
