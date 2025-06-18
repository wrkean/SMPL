#pragma once

#include <cstddef>
#include <string>

class Smpl {
public:
    static void error(size_t line, const std::string& msg);
};
