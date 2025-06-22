#pragma once

#include "smpl/types.hpp"
// #include <cstdint>    TODO: CLEAN
#include <string>
class TypeChecker {
public:
    static bool is_integer(SmplType type);
    static bool is_floating(SmplType type);
    static bool is_numeric(SmplType type);
    static bool are_assign_compatible(SmplType left, SmplType right);
    static bool are_binary_compatible(SmplType left, SmplType right);
    static bool fits_in_type(SmplType type, const std::string& literal);
    // static SmplType promote(SmplType a, SmplType b, int64_t line_number);
    static SmplType str_to_type(const std::string& type_str, size_t line_number);
};
