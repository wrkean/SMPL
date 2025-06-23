#pragma once

enum class ExprASTKind {
    Binary,
    Unary,
    BooleanLiteral,
    FnCall,
    Grouping,
    Identifier,
    NumberLiteral,
    Type,
};

enum class StmtASTKind {
    Assignment,
    Block,
    ExprStmt,
    FnDecl,
    For,
    If,
    Param,
    Return,
    While,
};
