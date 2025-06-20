#pragma once

#include "ast/expr/binary.hpp"
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
    SemanticAnalyzer(std::unique_ptr<StmtNode>& root);

    std::vector<std::unordered_map<std::string, Symbol>> symbol_table;
    std::unordered_map<std::string, SmplType> type_table;

    std::unique_ptr<StmtNode>& root;

    bool declare_symbol(const std::string& name, Symbol symbol);
    Symbol* lookup(const std::string& name);
    void enter_scope();
    void exit_scope();
    bool is_integer(SmplType type) const;
    bool is_floating(SmplType type) const;
    bool is_numeric(SmplType type) const;
    SmplType promote(SmplType a, SmplType b) const;
    SmplType str_to_type(const std::string& type_str) const;
    SmplType analyze_expr(std::unique_ptr<ExprNode>& stmt);
    void analyze_stmt(std::unique_ptr<StmtNode>& stmt);
};
