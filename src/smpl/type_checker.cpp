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
    {"int", SmplType::Int},
    {"uint", SmplType::Uint},
    {"f32", SmplType::Float32},
    {"f64", SmplType::Float64},
    {"bool", SmplType::Boolean},
    {"char", SmplType::Char}
};

bool TypeChecker::is_integer(SmplType type) {
    return type >= SmplType::Int8 && type <= SmplType::Uint;
}

bool TypeChecker::is_floating(SmplType type) {
    return type == SmplType::Float32 || type == SmplType::Float64;
}

bool TypeChecker::is_numeric(SmplType type) {
    return is_integer(type) || is_floating(type);
}

bool TypeChecker::are_assign_compatible(SmplType left, SmplType right) {
    if (left == right) return true;

    if ((right == SmplType::UntypedInt && is_integer(left)) || (right == SmplType::UntypedFloat && is_floating(left))) {
        return true;
    }

    // DOES NOT allow implicit conversions, unlike C
    return false;
}

bool TypeChecker::are_binary_compatible(SmplType left, SmplType right) {
    if (left == right) return true;

    // Handle untyped integer literal on either side
    if (left == SmplType::UntypedInt && is_integer(right)) return true;

    if (right == SmplType::UntypedInt && is_integer(left)) return true;

    // Handle untyped float literal on either side
    if (left == SmplType::UntypedFloat && is_floating(right)) return true;

    if (right == SmplType::UntypedFloat && is_floating(left)) return true;

    // DOES NOT allow implicit conversions as well
    return false;
}

bool TypeChecker::fits_in_type(SmplType type, const std::string& literal_value) {
    try {
        switch (type) {
            case SmplType::Int8: {
                long long val = std::stoll(literal_value);
                return val >= INT8_MIN && val <= INT8_MAX;
            }
            case SmplType::Int16: {
                long long val = std::stoll(literal_value);
                return val >= INT16_MIN && val <= INT16_MAX;
            }
            case SmplType::Int32: {
                long long val = std::stoll(literal_value);
                return val >= INT32_MIN && val <= INT32_MAX;
            }
            case SmplType::Int64: {
                std::stoll(literal_value); // Check if it's valid
                return true;
            }

            case SmplType::Uint8: {
                unsigned long long val = std::stoull(literal_value);
                return val <= UINT8_MAX;
            }
            case SmplType::Uint16: {
                unsigned long long val = std::stoull(literal_value);
                return val <= UINT16_MAX;
            }
            case SmplType::Uint32: {
                unsigned long long val = std::stoull(literal_value);
                return val <= UINT32_MAX;
            }
            case SmplType::Uint64: {
                unsigned long long val = std::stoull(literal_value);
                return val <= std::numeric_limits<uint64_t>::max();
            }

            case SmplType::Float32: {
                float val = std::stof(literal_value);
                return !std::isinf(val) && !std::isnan(val);
            }

            case SmplType::Float64: {
                double val = std::stod(literal_value);
                return !std::isinf(val) && !std::isnan(val);
            }

            default:
                return false;
        }
    } catch (const std::invalid_argument&) {
        return false; // Not a number
    } catch (const std::out_of_range&) {
        return false; // Too big or small to fit
    }
}

// TODO: Clean if not needed
// SmplType TypeChecker::promote(SmplType a, SmplType b, size_t line_number) {
//     if (!is_numeric(a)) throw TypeError("Invalid left hand type, it must be numerical", line_number);
//     if (!is_numeric(b)) throw TypeError("Invalid right hand type, it must be numerical", line_number);
//     // TODO: Better message
//     // FIXME: Might be buggy, or can be improved
//     if (!(is_integer(a) && is_integer(b)) && !(is_floating(a) && is_floating(b))) throw TypeError("Floating-point and integer operations not supported", line_number);
//     return (static_cast<int>(a) > static_cast<int>(b)) ? a : b;
// }

SmplType TypeChecker::str_to_type(const std::string& type_str, size_t line_number) {
    if (type_table.contains(type_str)) return type_table.at(type_str);
    throw TypeError("Invalid type", line_number);
}
