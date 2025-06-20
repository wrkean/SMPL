#include "semantic_analyzer/semantic_analyzer.hpp"
#include "ast/ast_kinds.hpp"
#include "ast/expr/binary.hpp"
#include "ast/expr/fncall.hpp"
#include "ast/expr/grouping.hpp"
#include "ast/expr/identifier.hpp"
#include "ast/expr/num_literal.hpp"
#include "ast/expr/unary.hpp"
#include "ast/stmt/assignment.hpp"
#include "ast/stmt/block.hpp"
#include "ast/stmt/expr_stmt.hpp"
#include "ast/stmt/fndecl.hpp"
#include "ast/stmt/for.hpp"
#include "ast/stmt/if.hpp"
#include "ast/stmt/param.hpp"
#include "ast/stmt/return.hpp"
#include "ast/stmt/while.hpp"
#include "error_reporter/compiler_err.hpp"
#include "smpl/smpl.hpp"
#include "smpl/symbol.hpp"
#include "smpl/type_checker.hpp"
#include "smpl/types.hpp"
#include "token/tokenkind.hpp"
#include <cstdlib>
#include <memory>
#include <vector>

// TODO: Finish and clean these up

SemanticAnalyzer::SemanticAnalyzer(std::vector<std::unique_ptr<StmtNode>>& program)
    : program(program), cur_func_return_type(SmplType::Unknown) { };

bool SemanticAnalyzer::declare_symbol(const std::string& name, Symbol symbol) {
    if (symbol_table.empty()) throw CompilerError("No scope available for declaration", symbol.token.line);
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

void SemanticAnalyzer::analyze() {
    enter_scope();
    for (auto& statement : program) {
        try {
            analyze_stmt(statement);
        } catch (const CompilerError& err) {
            Smpl::error(err.line_number, err.what());
        }
    }
    exit_scope();
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
                        return TypeChecker::promote(left_type, right_type, node->op.line);
                    }
                    throw TypeError("Both operands must be integers", node->op.line);
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
                        return TypeChecker::promote(left_type, right_type, node->op.line);
                    }
                    if (left_type == right_type) {
                        return left_type;
                    }
                    throw TypeError("Operations on different types is not allowed", node->op.line);
                case TokenKind::EqualEqual:
                case TokenKind::NotEqual:
                    if (left_type == SmplType::Boolean && right_type == SmplType::Boolean) {
                        return SmplType::Boolean;
                    }
                    throw TypeError("Equality operators only accepts booleans as operands", node->op.line);
                case TokenKind::Or:
                case TokenKind::And:
                    if (left_type == SmplType::Boolean && right_type == SmplType::Boolean) {
                        return SmplType::Boolean;
                    }
                    throw TypeError("Logical operators only accepts booleans as operands", node->op.line);
                case TokenKind::LesserThan:
                case TokenKind::LesserEqual:
                case TokenKind::GreaterThan:
                case TokenKind::GreaterEqual:
                    if (TypeChecker::is_numeric(left_type) && TypeChecker::is_numeric(right_type)) {
                        return TypeChecker::promote(left_type, right_type, node->op.line);
                    }
                    throw TypeError("Relational operators only accepts numeric values as operands", node->op.line);
                default:
                    throw LexicalError("Unrecognized operator " + node->op.lexeme, node->op.line);
            }
        }
        case ExprASTKind::NumberLiteral: {
            NumLitNode* node = dynamic_cast<NumLitNode*>(expr.get());
            Token literal = node->literal;
            return (literal.lexeme.contains(".")) ? SmplType::Float64 : SmplType::Int32;
        }
        case ExprASTKind::Identifier: {
            IdentifierNode* node = dynamic_cast<IdentifierNode*>(expr.get());
            Symbol* symbol = lookup(node->identifier.lexeme);
            if (!symbol) {
                throw CompilerError("Undefined variable", node->identifier.line);
            }

            return symbol->type;
        }
        case ExprASTKind::FnCall: {
            FuncCallNode* node = dynamic_cast<FuncCallNode*>(expr.get());
            Symbol* symbol = lookup(node->identifier.lexeme);
            if (!symbol) {
                throw CompilerError("Undefined function", node->identifier.line);
            }

            if (!symbol->is_function) {
                throw TypeError(symbol->token.lexeme + " is not a function", node->identifier.line);
            }

            const auto& param_types = symbol->param_types;
            if (param_types.size() != node->args.size()) {
                throw TypeError("Incorrect number of arguments", node->identifier.line);
            }

            for (int i = 0; i < param_types.size(); i++) {
                SmplType arg_type = analyze_expr(node->args[i]);
                if (TypeChecker::is_numeric(arg_type) && TypeChecker::is_numeric(param_types[i])) {
                    TypeChecker::promote(arg_type, param_types[i], node->identifier.line);
                }
                // TODO: Better error message
                if (arg_type != param_types[i]) {
                    throw TypeError("Incompatible type passed as argument", node->identifier.line);
                }
            }
            return symbol->type;
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
                    throw TypeError("Unary 'not' operator only accepts a boolean as an operand", node->op.line);
                case TokenKind::Minus:
                    if (TypeChecker::is_numeric(right_type)) {
                        return right_type;
                    }
                    throw TypeError("Unary '-' operator only accepts a numerical value as an operand", node->op.line);
                default:
                    throw LexicalError("Unrecognized operator " + node->op.lexeme, node->op.line);
            }
        }
    }
}

void SemanticAnalyzer::analyze_stmt(std::unique_ptr<StmtNode>& stmt) {
    switch (stmt->get_kind()) {
        case StmtASTKind::Assignment: {
            AssignmentNode* node = dynamic_cast<AssignmentNode*>(stmt.get());
            SmplType type = TypeChecker::str_to_type(node->var_type.lexeme, node->var_type.line);

            if (!declare_symbol(node->variable.lexeme, Symbol(type, node->variable)))
                throw CompilerError("Redeclaration of variable", node->variable.line);

            SmplType right_type = analyze_expr(node->right);

            if (TypeChecker::is_numeric(type) && TypeChecker::is_numeric(right_type)) {
                TypeChecker::promote(type, right_type, node->var_type.line);
            } else if (type != right_type) {
                // TODO: Better error message
                throw TypeError("Incompatible types", node->var_type.line);
            }
        }
        break;
        case StmtASTKind::Block: {
            enter_scope();
            BlockNode* node = dynamic_cast<BlockNode*>(stmt.get());
            for (auto& stmt : node->statements) {
                analyze_stmt(stmt);
            }
            exit_scope();
        }
        break;
        case StmtASTKind::ExprStmt: {
            ExprStmt* node = dynamic_cast<ExprStmt*>(stmt.get());
            analyze_expr(node->expr);
        }
        break;
        case StmtASTKind::FnDecl: {
            DefnNode* node = dynamic_cast<DefnNode*>(stmt.get());
            SmplType type;
            if (node->return_type.has_value()) {
                type = TypeChecker::str_to_type(node->return_type->lexeme, node->return_type->line);
            } else {
                type = SmplType::Void;
            }
            cur_func_return_type = type;

            // Get param types
            std::vector<SmplType> param_types;
            ParamNode* param_node = dynamic_cast<ParamNode*>(node->params.get());
            for (auto& param : param_node->params) {
                param_types.push_back(TypeChecker::str_to_type(param.second.lexeme, node->identifier.line));
            }
            Symbol fn_sym(type, param_types, node->identifier);
            if (!declare_symbol(node->identifier.lexeme, fn_sym)) {
                // TODO: Better error message
                throw CompilerError("Redeclaration of the variable", node->identifier.line);
            }
            enter_scope();
            analyze_stmt(node->params);
            analyze_stmt(node->block);
            exit_scope();
        }
        break;
        case StmtASTKind::For: {
            enter_scope();
            ForNode* node = dynamic_cast<ForNode*>(stmt.get());
            SmplType bind_var_type = analyze_expr(node->iterator);
            declare_symbol(node->bind_var.lexeme, Symbol(bind_var_type, node->bind_var));
            analyze_stmt(node->block);
            exit_scope();
        }
        break;
        case StmtASTKind::If: {
            IfNode* node = dynamic_cast<IfNode*>(stmt.get());
            for (auto& branch : node->branches) {
                enter_scope();
                if (!branch.condition) {
                    analyze_stmt(branch.block);
                    break;
                }
                analyze_expr(branch.condition);
                analyze_stmt(branch.block);
                exit_scope();
            }
        }
        break;
        case StmtASTKind::Param: {
            ParamNode* params = dynamic_cast<ParamNode*>(stmt.get());
            for (auto& param : params->params) {
                SmplType param_type = TypeChecker::str_to_type(param.second.lexeme, param.second.line);
                declare_symbol(param.first.lexeme, Symbol(param_type, param.first));
            }
        }
        break;
        case StmtASTKind::Return: {
            ReturnNode* node = dynamic_cast<ReturnNode*>(stmt.get());
            if (cur_func_return_type == SmplType::Unknown) {
                throw CompilerError("Returning outside the function is not allowed", -1);
            }
            SmplType type = analyze_expr(node->return_expr);
            TypeChecker::promote(cur_func_return_type, type, -1);
        }
        break;
        case StmtASTKind::While: {
            enter_scope();
            WhileNode* node = dynamic_cast<WhileNode*>(stmt.get());
            analyze_expr(node->condition);
            analyze_stmt(node->block);
        }
    }
}
