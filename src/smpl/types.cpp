#include "smpl/types.hpp"
#include "error_reporter/compiler_err.hpp"
#include <string>
#include <unordered_map>

SmplType str_to_type(const std::string& id) {
    std::unordered_map<std::string, SmplType> type_map = {
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
        {"f16", SmplType::Float16},
        {"f32", SmplType::Float32},
        {"f64", SmplType::Float64},
        {"f128", SmplType::Float128},
        {"bool", SmplType::Boolean},
        // {"str", SmplType::String}, // TODO: Support for string types later
        {"char", SmplType::Char},
    };

    if (type_map.contains(id)) {
        return type_map.at(id);
    }

    throw TypeError("Invalid type");
}
