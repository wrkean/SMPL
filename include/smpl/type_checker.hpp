#pragma once

#include "smpl/types.hpp"
#include <cstdint>
#include <string>
class TypeChecker {
public:
    static bool is_integer(SmplType type);
    static bool is_floating(SmplType type);
    static bool is_numeric(SmplType type);
    static SmplType promote(SmplType a, SmplType b, int64_t line_number);
    static SmplType str_to_type(const std::string& type_str, int64_t line_number);
};
