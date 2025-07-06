#pragma once

#include <cstddef>
#include <string>

class Smpl {
public:
    static void error(size_t line, const std::string& msg);
    static std::string file_to_strbuf(const std::string& path);
};
