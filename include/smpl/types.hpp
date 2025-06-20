#pragma once

#include <string>
enum class SmplType {
    Int8, Uint8,
    Int16, Uint16,
    Int32, Uint32,
    Int64, Uint64,
    Int, Uint,

    Float16, Float32,
    Float64, Float128,

    Boolean,

    String,
    Char,

    Void,
    Unknown,
};

SmplType str_to_type(const std::string& id);
