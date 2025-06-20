#include "semantic_analyzer/semantic_analyzer.hpp"
#include "ast/ast_kinds.hpp"
#include "ast/expr/binary.hpp"
#include "ast/stmt/assignment.hpp"
#include "error_reporter/compiler_err.hpp"
#include "smpl/symbol.hpp"
#include "smpl/types.hpp"
#include <memory>

// TODO: Finish and clean these up
bool SemanticAnalyzer::declare_symbol(const std::string& name, Symbol symbol) {
    if (symbol_table.empty()) throw CompilerError("No scope available for declaration");
    if (symbol_table.back().contains(name)) return false;   // Cant declare, already declared

    symbol_table.back().emplace(name, symbol);
    return true;
}

Symbol* SemanticAnalyzer::lookup(const std::string& name) {
    for (auto it = symbol_table.rbegin(); it != symbol_table.rend(); ++it) {
        if (it->contains(name)) {
            return &it->at(name);
        }
    }
    return nullptr;
}

void SemanticAnalyzer::enter_scope() {
    symbol_table.push_back({});
}

void SemanticAnalyzer::exit_scope() {
    symbol_table.pop_back();
}

// TODO: Implement analyze_expr
SmplType SemanticAnalyzer::analyze_expr(std::unique_ptr<ExprNode>& expr) {
    switch (expr->get_kind()) {
        case ExprASTKind::Binary: {
            BinaryExpr* node = dynamic_cast<BinaryExpr*>(expr.get());
            SmplType type = node->get_type();
            if (type == SmplType::Unknown) {
            }
        }
    }
}

void SemanticAnalyzer::analyze_stmt(std::unique_ptr<StmtNode>& stmt) {
    switch (stmt->get_kind()) {
        case StmtASTKind::Assignment: {
            AssignmentNode* node = dynamic_cast<AssignmentNode*>(stmt.get());
            SmplType type = str_to_type(node->var_type.lexeme);

            if (!declare_symbol(node->variable.lexeme, Symbol(type, node->variable)))
                throw CompilerError("Redeclaration of variable");

            SmplType right_type = analyze_expr(node->right);

            // TODO: More detailed error message for incompatible types
            if (type != right_type) throw TypeError("Incompatible types for assignment");
        }
    }
}
