#pragma once

#include "smpl/types.hpp"
// #include <cstdint>    TODO: CLEAN
#include <string>
#include <unordered_map>

namespace tc {
    extern std::unordered_map<std::string, SmplType> type_table;
    bool is_integer(SmplType type);
    bool is_floating(SmplType type);
    bool is_numeric(SmplType type);
    bool are_assign_compatible(SmplType left, SmplType right);
    bool are_binary_compatible(SmplType left, SmplType right);
    bool fits_in_type(SmplType type, const std::string& literal);
    bool is_type(const std::string& name);
    bool is_castable(SmplType from, SmplType to);
    SmplType str_to_type(const std::string& type_str, size_t line_number);
};
