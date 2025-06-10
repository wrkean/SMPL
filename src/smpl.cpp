#include "smpl.hpp"
#include <cstdio>
#include <iostream>

void Smpl::error(size_t line, const std::string& msg) {
    std::cout << "line " << "[" << line << "]: " << msg << "\n";
}
