#pragma once

#include "ast/expr/expr.hpp"
#include "ast/stmt/stmt.hpp"
#include "smpl/symbol/symbol.hpp"
#include "smpl/types.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class SemanticAnalyzer {
public:
    SemanticAnalyzer(std::vector<std::unique_ptr<StmtNode>>& program);

    std::vector<std::unordered_map<std::string, std::unique_ptr<Symbol>>> symbol_table;

    std::vector<std::unique_ptr<StmtNode>>& program;
    SmplType cur_func_return_type;

    bool declare_symbol(const std::string& name, std::unique_ptr<Symbol> symbol);
    Symbol* lookup(const std::string& name);
    void enter_scope();
    void exit_scope();
    SmplType to_type(const std::string& type_str) const;
    bool analyze();
    SmplType analyze_expr(std::unique_ptr<ExprNode>& stmt);
    void analyze_stmt(std::unique_ptr<StmtNode>& stmt);
};
