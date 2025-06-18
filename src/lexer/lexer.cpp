#include "lexer/lexer.hpp"
#include "error_reporter/compiler_err.hpp"
#include "token/tokenkind.hpp"
#include <cctype>
#include <format>
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
    keywords.emplace("else", TokenKind::Else);
    keywords.emplace("while", TokenKind::While);
    keywords.emplace("true", TokenKind::True);
    keywords.emplace("false", TokenKind::False);
    keywords.emplace("not", TokenKind::Not);
    keywords.emplace("and", TokenKind::And);
    keywords.emplace("or", TokenKind::Or);
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
            } else if (match('/')) {
                // Ignore comments
                while (!at_end() && peek() != '\n') {
                    advance();
                }
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
        case '%':
            if (match('=')) {
                add_token(TokenKind::PercentEqual);
            } else {
                add_token(TokenKind::Percent);
            }
            break;
        case '>':
            if (match('=')) {
                add_token(TokenKind::GreaterEqual);
            } else if (match('>')) {
                if (match('=')) {
                    add_token(TokenKind::GreaterGreaterEqual);
                } else {
                    add_token(TokenKind::GreaterGreater);
                }
            } else {
                add_token(TokenKind::GreaterThan);
            }
            break;
        case '<':
            if (match('=')) {
                add_token(TokenKind::LesserEqual);
            } else if (match('<')) {
                if (match('=')) {
                    add_token(TokenKind::LesserLesserEqual);
                } else {
                    add_token(TokenKind::LesserLesser);
                }
            } else {
                add_token(TokenKind::LesserThan);
            }
            break;
        case '!':
            if (match('=')) {
                add_token(TokenKind::NotEqual);
            } else {
                add_token(TokenKind::Not);
            }
            break;
        case '&':
            if (match('=')) {
                add_token(TokenKind::AmperEqual);
            } else if (match('&')) {
                add_token(TokenKind::And);
            } else {
                add_token(TokenKind::Ampersand);
            }
            break;
        case '|':
            if (match('=')) {
                add_token(TokenKind::PipeEqual);
            } else if (match('|')) {
                add_token(TokenKind::Or);
            } else {
                add_token(TokenKind::Pipe);
            }
            break;
        case '^':
            if (match('=')) {
                add_token(TokenKind::CaretEqual);
            } else {
                add_token(TokenKind::Caret);
            }
            break;
        case '~':
            if (match('=')) {
                add_token(TokenKind::TildeEqual);
            } else {
                add_token(TokenKind::Tilde);
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

    if (isalnum(peek())) {
        while (!at_end() && isalnum(peek())) {
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
