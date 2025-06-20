#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

struct CompilerError : public std::runtime_error {
    int64_t line_number;
    CompilerError(const std::string& message, int line_number)
        : std::runtime_error(message), line_number(line_number) {}
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
