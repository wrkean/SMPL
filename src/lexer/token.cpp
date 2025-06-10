#include "lexer/token.hpp"
#include "lexer/tokenkind.hpp"

Token::Token(TokenKind kind, const std::string& lexeme, size_t line)
    : kind(kind), lexeme(lexeme), line(line) { }
