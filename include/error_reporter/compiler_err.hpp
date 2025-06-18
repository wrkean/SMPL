#pragma once

#include <stdexcept>

struct CompilerError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct SyntaxError : public CompilerError {
    using CompilerError::CompilerError;
};

struct LexicalError : public CompilerError {
    using CompilerError::CompilerError;
};

struct TypeError : public CompilerError {
    using CompilerError::CompilerError;
};
