#include "lexer/lexer.hpp"
#include "error_reporter/compiler_err.hpp"
#include "token/tokenkind.hpp"
#include "smpl.hpp"
#include <format>
#include <stdexcept>
#include <string>
#include <vector>

Lexer::Lexer(const std::string&& source)
    : source(std::move(source)), line(1), start(0), current(0)
{
    keywords.emplace("defn", TokenKind::Defn);
    keywords.emplace("let", TokenKind::Let);
    keywords.emplace("as", TokenKind::As);
    keywords.emplace("return", TokenKind::Return);
    keywords.emplace("in", TokenKind::In);
    keywords.emplace("for", TokenKind::For);
    keywords.emplace("if", TokenKind::If);
    keywords.emplace("while", TokenKind::While);
}

std::vector<Token>& Lexer::lex() {
    while (!at_end()) {
        start = current;
        lex_token();
    }

    add_token(TokenKind::Eof);
    return tokens;
}

void Lexer::lex_token() {
    char ch = advance();

    switch (ch) {
        case '(': add_token(TokenKind::LeftParen); break;
        case ')': add_token(TokenKind::RightParen); break;
        case '{': add_token(TokenKind::LeftBrace); break;
        case '}': add_token(TokenKind::RightBrace); break;
        case ':': add_token(TokenKind::Colon); break;
        case ';': add_token(TokenKind::SemiColon); break;
        case ',': add_token(TokenKind::Comma); break;
        case '=':
            if (match('=')) {
                add_token(TokenKind::EqualEqual);
            } else {
                add_token(TokenKind::Equal);
            }
            break;
        case '+':
            if (match('=')) {
                add_token(TokenKind::PlusEqual);
            } else {
                add_token(TokenKind::Plus);
            }
            break;
        case '-':
            if (match('=')) {
                add_token(TokenKind::MinusEqual);
            } else if (match('>')) {
                add_token(TokenKind::ThinArrow);
            } else {
                add_token(TokenKind::Minus);
            }
            break;
        case '*':
            if (match('=')) {
                add_token(TokenKind::StarEqual);
            } else {
                add_token(TokenKind::Star);
            }
            break;
        case '/':
            if (match('=')) {
                add_token(TokenKind::ForSlashEqual);
            } else {
                add_token(TokenKind::ForSlash);
            }
            break;
        case '.':
            if (match('.')) {
                add_token(TokenKind::Range);
            } else {
                // TODO: Add dot token
            }
            break;
        // Lex string literals
        case '"':
            lex_string(); break;
        // Ignore whitespaces
        case ' ':
        case '\t':
        case '\r':
            break;
        // Check for newlines
        case '\n': line++; break;
        // TODO: lex char literals
        // case '\'':
        //     lex_char(); break;
        default:
            if (isalpha(ch)) {
                lex_identifier();
            } else if (isdigit(ch)) {
                lex_number();
            } else {
                std::string msg = std::format("[{}]: Unexpected character: {}", line, ch);
                throw LexicalError(msg);
            }
            break;
    }
} // !lex_token

void Lexer::add_token(TokenKind kind) {
    std::string lexeme = source.substr(start, current - start);
    add_token(kind, lexeme);
}

void Lexer::add_token(TokenKind kind, const std::string& lexeme) {
    tokens.emplace_back(kind, lexeme, line);
}

void Lexer::lex_string() {
    while (peek() != '"' && peek() != '\n' && !at_end()) {
        advance();
    }

    if (at_end() || peek() == '\n') {
        throw LexicalError(std::format("[{}]: Unterminated string literal", line));
    }

    std::string lexeme = source.substr(start + 1, current - start - 1);
    add_token(TokenKind::StringLiteral, lexeme);
}

void Lexer::lex_identifier() {
    while (!at_end() && (isalnum(peek()) || peek() == '_')) {
        advance();
    }

    std::string id = source.substr(start, current - start);

    if (keywords.contains(id)) {
        add_token(keywords.at(id));
        return;
    }

    add_token(TokenKind::Identifier);
}

void Lexer::lex_number() {
    while (!at_end() && (isdigit(peek()) || peek() == '_')) {
        advance();
    }

    if (peek() == '.' && isdigit(peek_next())) {
        advance();  // Consume '.'
        while (!at_end() && (isdigit(peek()))) {
            advance();
        }
    }

    std::string num;
    for (int i = start; i < current; i++) {
        // Strip off underlines
        if (source[i] != '_') {
            num += source[i];
        }
    }

    add_token(TokenKind::NumLiteral, num);
}

char Lexer::advance() {
    current++;
    return source[current - 1];
}

char Lexer::peek() const {
    return source[current];
}

char Lexer::peek_next() const {
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (peek() != expected) return false;

    current++;
    return true;
}

bool Lexer::at_end() const {
    return current >= source.length();
}
