#pragma once

#include <string>
#include <unordered_map>

class SemanticAnalyzer {
public:
    std::vector<std::unordered_map<std::string, Symbol>> symbol_table;

    void declare_symbol();
    Symbol* lookup();
    void enter_scope();
    void exit_scope();
};
