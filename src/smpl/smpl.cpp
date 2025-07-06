#include "smpl/smpl.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

void Smpl::error(size_t line, const std::string& msg) {
    std::cout << "line " << "[" << line << "]: " << msg << "\n";
}

std::string file_to_strbuf(const std::string& path) {
    std::ifstream source(path);
    if (!source.is_open()) {
        std::cerr << "Could not open file\n";
        return "";
    }

    std::stringstream buf;
    buf << source.rdbuf();

    return buf.str();
}
