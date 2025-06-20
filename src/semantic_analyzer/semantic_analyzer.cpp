#include "semantic_analyzer/semantic_analyzer.hpp"
#include "ast/ast_kinds.hpp"
#include "ast/expr/binary.hpp"
#include "ast/expr/fncall.hpp"
#include "ast/expr/grouping.hpp"
#include "ast/expr/identifier.hpp"
#include "ast/expr/num_literal.hpp"
#include "ast/expr/unary.hpp"
#include "ast/stmt/assignment.hpp"
#include "error_reporter/compiler_err.hpp"
#include "smpl/symbol.hpp"
#include "smpl/type_checker.hpp"
#include "smpl/types.hpp"
#include "token/tokenkind.hpp"
#include <memory>

// TODO: Finish and clean these up

SemanticAnalyzer::SemanticAnalyzer(std::unique_ptr<StmtNode>& root)
    : root(root) { }

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
            SmplType left_type = analyze_expr(node->left);
            SmplType right_type = analyze_expr(node->right);

            switch (node->op.kind) {
                case TokenKind::As: return right_type;
                case TokenKind::Range:
                    if (TypeChecker::is_integer(left_type) && TypeChecker::is_integer(right_type)) {
                        return TypeChecker::promote(left_type, right_type);
                    }
                    throw TypeError("Both operands must be integers");
                case TokenKind::Equal:
                case TokenKind::Plus:
                case TokenKind::PlusEqual:
                case TokenKind::Minus:
                case TokenKind::MinusEqual:
                case TokenKind::Star:
                case TokenKind::StarEqual:
                case TokenKind::ForSlash:
                case TokenKind::ForSlashEqual:
                case TokenKind::Percent:
                case TokenKind::PercentEqual:
                    // NOTE: Might be redundant
                    if (node->right->get_kind() == ExprASTKind::NumberLiteral) {
                        return TypeChecker::promote(left_type, right_type);
                    }
                    if (left_type == right_type) {
                        return left_type;
                    }
                    throw TypeError("Operations on different types is not allowed");
                case TokenKind::Or:
                case TokenKind::And:
                    if (left_type == SmplType::Boolean && right_type == SmplType::Boolean) {
                        return SmplType::Boolean;
                    }
                    throw TypeError("Logical operators only accepts booleans as operands");
                case TokenKind::EqualEqual:
                case TokenKind::NotEqual:
                case TokenKind::LesserThan:
                case TokenKind::LesserEqual:
                case TokenKind::GreaterThan:
                case TokenKind::GreaterEqual:
                    if (TypeChecker::is_numeric(left_type) && TypeChecker::is_numeric(right_type)) {
                        return TypeChecker::promote(left_type, right_type);
                    }
                    throw TypeError("Relational operators only accepts numeric values as operands");
                default:
                    throw LexicalError("Unrecognized operator " + node->op.lexeme);
            }
        }
        case ExprASTKind::NumberLiteral: {
            NumLitNode* node = dynamic_cast<NumLitNode*>(expr.get());
            Token literal = node->literal;
            return (literal.lexeme.contains(".")) ? SmplType::Int32 : SmplType::Float64;
        }
        case ExprASTKind::Identifier: {
            IdentifierNode* node = dynamic_cast<IdentifierNode*>(expr.get());
            Symbol* symbol = lookup(node->identifier.lexeme);
            if (!symbol) {
                throw CompilerError("Undefined variable");
            }

            return symbol->type;
        }
        // FIXME: Function call type check
        case ExprASTKind::FnCall: {
            FuncCallNode* node = dynamic_cast<FuncCallNode*>(expr.get());
            Symbol* symbol = lookup(node->identifier.lexeme);
            if (!symbol) {
                throw CompilerError("Undefined function");
            }
        }
        case ExprASTKind::Grouping: {
            GroupingExpr* node = dynamic_cast<GroupingExpr*>(expr.get());
            return analyze_expr(node->expr);
        }
        case ExprASTKind::BooleanLiteral: return SmplType::Boolean;
        case ExprASTKind::Unary: {
            UnaryNode* node = dynamic_cast<UnaryNode*>(expr.get());
            SmplType right_type = analyze_expr(node->right);
            switch (node->op.kind) {
                case TokenKind::Not:
                    if (right_type == SmplType::Boolean) {
                        return right_type;
                    }
                    throw TypeError("Unary 'not' operator only accepts a boolean as an operand");
                case TokenKind::Minus:
                    if (TypeChecker::is_numeric(right_type)) {
                        return right_type;
                    }
                    throw TypeError("Unary '-' operator only accepts a numerical value as an operand");
                default:
                    throw LexicalError("Unrecognized operator " + node->op.lexeme);
            }
        }
    }
}

void SemanticAnalyzer::analyze_stmt(std::unique_ptr<StmtNode>& stmt) {
    switch (stmt->get_kind()) {
        case StmtASTKind::Assignment: {
            AssignmentNode* node = dynamic_cast<AssignmentNode*>(stmt.get());
            SmplType type = TypeChecker::str_to_type(node->var_type.lexeme);

            if (!declare_symbol(node->variable.lexeme, Symbol(type, node->variable)))
                throw CompilerError("Redeclaration of variable");

            SmplType right_type = analyze_expr(node->right);

            // TODO: More detailed error message for incompatible types
            if (type != right_type) throw TypeError("Incompatible types for assignment");
        }
    }
}
