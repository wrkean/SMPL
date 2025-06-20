#pragma once

#include "smpl/symbol.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class SemanticAnalyzer {
public:
    std::vector<std::unordered_map<std::string, Symbol>> symbol_table;

    bool declare_symbol(const std::string& name, Symbol symbol);
    Symbol* lookup(const std::string& name);
    void enter_scope();
    void exit_scope();
};
