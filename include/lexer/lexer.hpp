#pragma once

#include "token/token.hpp"
#include "token/tokenkind.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class Lexer {
public:
    Lexer(const std::string&& source);
    std::vector<Token>& lex();

private:
    std::string source;
    std::unordered_map<std::string, TokenKind> keywords;
    std::vector<Token> tokens;
    size_t line;
    size_t start;
    size_t current;

    // Private methods
    void lex_token();
    void lex_string();
    void lex_identifier();
    void lex_number();
    void add_token(TokenKind kind);
    void add_token(TokenKind kind, const std::string& token);

    // Helper methods
    char advance();
    char peek() const;
    char peek_next() const;
    bool match(char expected);
    bool at_end() const;
};
