#include "smpl/type_checker.hpp"
#include "error_reporter/compiler_err.hpp"
#include "smpl/types.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>

static std::unordered_map<std::string, SmplType> type_table = {
    {"i8", SmplType::Int8},
    {"u8", SmplType::Uint8},
    {"i16", SmplType::Int16},
    {"u16", SmplType::Uint16},
    {"i32", SmplType::Int32},
    {"u32", SmplType::Uint32},
    {"i64", SmplType::Int64},
    {"u64", SmplType::Uint64},
    {"f16", SmplType::Float16},
    {"f32", SmplType::Float32},
    {"f64", SmplType::Float64},
    {"f128", SmplType::Float128},
    {"bool", SmplType::Boolean},
    {"char", SmplType::Char}
};

bool TypeChecker::is_integer(SmplType type) {
    return type >= SmplType::Int8 && type <= SmplType::Uint;
}

bool TypeChecker::is_floating(SmplType type) {
    return type >= SmplType::Float16 && type <= SmplType::Float128;
}

bool TypeChecker::is_numeric(SmplType type) {
    return is_integer(type) || is_floating(type);
}

SmplType TypeChecker::promote(SmplType a, SmplType b, int64_t line_number) {
    if (!is_numeric(a)) throw TypeError("Invalid left hand type, it must be numerical", line_number);
    if (!is_numeric(b)) throw TypeError("Invalid right hand type, it must be numerical", line_number);
    // TODO: Better message
    // FIXME: Might be buggy, or can be improved
    if (!(is_integer(a) && is_integer(b)) && !(is_floating(a) && is_floating(b))) throw TypeError("Floating-point and integer operations not supported", line_number);
    return (static_cast<int>(a) > static_cast<int>(b)) ? a : b;
}

SmplType TypeChecker::str_to_type(const std::string& type_str, int64_t line_number) {
    if (type_table.contains(type_str)) return type_table.at(type_str);
    throw TypeError("Invalid type", line_number);
}
