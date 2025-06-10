#pragma once

enum class TokenKind {
    Defn,
    Let,
    As,
    For,
    Return,

    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    Colon,
    SemiColon,
    Comma,
    ThinArrow,
    Range,
    Plus,
    PlusEqual,
    Minus,
    MinusEqual,
    Star,
    StarEqual,
    Equal,
    EqualEqual,
    ForSlash,
    ForSlashEqual,

    Identifier,
    NumLiteral,
    StringLiteral,
    CharLiteral,

    Eof,
};
