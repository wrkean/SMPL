#pragma once

#include "token/tokenkind.hpp"
#include <cstddef>
#include <string>

struct Token {
    Token(TokenKind kind, const std::string& lexeme, size_t line);

    TokenKind kind;
    std::string lexeme;
    size_t line;
};
