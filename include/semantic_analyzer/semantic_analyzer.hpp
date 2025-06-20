#pragma once

#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include "smpl/symbol.hpp"
#include "smpl/types.hpp"
#include <memory>
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
    SmplType analyze_expr(std::unique_ptr<ExprNode>& stmt);
    void analyze_stmt(std::unique_ptr<StmtNode>& stmt);
};
