#include "semantic_analyzer/semantic_analyzer.hpp"
#include "smpl/symbol.hpp"

// TODO: Finish and clean these up
bool SemanticAnalyzer::declare_symbol(const std::string& name, Symbol symbol) {
    if (symbol_table.back().contains(name)) return false;   // Cant declare, already declared

    symbol_table.back().emplace(name, symbol);
    return true;
}

Symbol* SemanticAnalyzer::lookup(const std::string& name) {
    if (symbol_table.back().contains(name)) return &symbol_table.back().at(name);
}
