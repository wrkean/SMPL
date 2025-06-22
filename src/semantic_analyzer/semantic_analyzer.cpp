#include "semantic_analyzer/semantic_analyzer.hpp"
#include "ast/ast_kinds.hpp"
#include "ast/expr/binary.hpp"
#include "ast/expr/expr.hpp"
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
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
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

void SemanticAnalyzer::analyze_stmt(std::unique_ptr<StmtNode>& stmt) {
    switch (stmt->get_kind()) {
        case StmtASTKind::Assignment: {
            AssignmentNode* assignment = dynamic_cast<AssignmentNode*>(stmt.get());
            SmplType ass_type = TypeChecker::str_to_type(assignment->var_type.lexeme, stmt->get_line());
            SmplType right_type = analyze_expr(assignment->right);
            //       ^^^ as in assignment
            // TODO: Better error report
            if (!TypeChecker::are_assign_compatible(ass_type, right_type)) {
                throw TypeError("Incompatible types for assignment", stmt->get_line());
            } else {
                if (assignment->right->get_kind() == ExprASTKind::NumberLiteral) {
                    NumLitNode* numlit = dynamic_cast<NumLitNode*>(assignment->right.get());
                    if (!TypeChecker::fits_in_type(ass_type, numlit->literal.lexeme)) {
                        throw CompilerError("An overflow occured", numlit->get_line());
                    }
                }
            }
            if (!declare_symbol(assignment->variable.lexeme, Symbol(ass_type, assignment->variable))) {
                throw CompilerError(std::format("{} already defined", assignment->variable.lexeme), stmt->get_line());
            }
        } break;
        case StmtASTKind::Block: {
            enter_scope();
            BlockNode* block = dynamic_cast<BlockNode*>(stmt.get());
            for (auto& block_statement : block->statements) {
                analyze_stmt(block_statement);
            }
            exit_scope();
        } break;
        case StmtASTKind::ExprStmt: {
            ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt.get());
            analyze_expr(expr_stmt->expr);
        } break;
        case StmtASTKind::FnDecl: {
            DefnNode* defn = dynamic_cast<DefnNode*>(stmt.get());
            SmplType return_type = SmplType::Void;
            if (defn->return_type.has_value()) {
                return_type = TypeChecker::str_to_type(defn->return_type->lexeme, defn->get_line());
            }
            cur_func_return_type = return_type;

            std::vector<SmplType> param_types;
            ParamNode* param_node = dynamic_cast<ParamNode*>(defn->params.get());
            for (auto& param : param_node->params) {
                SmplType param_type = TypeChecker::str_to_type(param.second.lexeme, param_node->get_line());
                param_types.push_back(param_type);
            }

            if (!declare_symbol(defn->identifier.lexeme, Symbol(return_type, param_types, defn->identifier))) {
                throw CompilerError(std::format("{} is already defined", defn->identifier.lexeme), defn->get_line());
            }

            enter_scope();
            analyze_stmt(defn->params);
            analyze_stmt(defn->block);
            exit_scope();
            cur_func_return_type = SmplType::Unknown;
        } break;
        case StmtASTKind::For: {
            ForNode* for_node = dynamic_cast<ForNode*>(stmt.get());
            SmplType type = analyze_expr(for_node->iterator);
            enter_scope();
            declare_symbol(for_node->bind_var.lexeme, Symbol(type, for_node->bind_var));
            analyze_stmt(for_node->block);
            exit_scope();
        } break;
        case StmtASTKind::If: {
            IfNode* if_node = dynamic_cast<IfNode*>(stmt.get());
            for (auto& branch : if_node->branches) {
                if (branch.condition != nullptr) {
                    analyze_expr(branch.condition);
                }
                analyze_stmt(branch.block);
            }
        } break;
        case StmtASTKind::Param: {
            ParamNode* param = dynamic_cast<ParamNode*>(stmt.get());
            for (auto& param_ : param->params) {
                SmplType type = TypeChecker::str_to_type(param_.second.lexeme, param->get_line());
                declare_symbol(param_.first.lexeme, Symbol(type, param_.first));
            }
        } break;
        case StmtASTKind::Return: {
            ReturnNode* return_node = dynamic_cast<ReturnNode*>(stmt.get());
            SmplType return_type = analyze_expr(return_node->return_expr);
            if (!TypeChecker::are_assign_compatible(cur_func_return_type, return_type)) {
                throw TypeError("");
            }
        } break;
        case StmtASTKind::While: {
            WhileNode* while_node = dynamic_cast<WhileNode*>(stmt.get());
            analyze_expr(while_node->condition);
            analyze_stmt(while_node->block);
        } break;
    }
}

SmplType SemanticAnalyzer::analyze_expr(std::unique_ptr<ExprNode>& expr) {
    switch (expr->get_kind()) {
        case ExprASTKind::Binary: {
            BinaryExpr* binary = dynamic_cast<BinaryExpr*>(expr.get());
            SmplType left_type = analyze_expr(binary->left);
            SmplType right_type = analyze_expr(binary->right);

            if (!TypeChecker::are_binary_compatible(left_type, right_type)) {
                std::ostringstream left_type_str, right_type_str;
                left_type_str << magic_enum::enum_name(left_type);
                right_type_str << magic_enum::enum_name(right_type);
                throw TypeError(std::format("Incompatible types for binary operation: {} and {}", left_type_str.str(), right_type_str.str()), binary->get_line());
            }

            switch (binary->op.kind) {
                case TokenKind::Plus:
                case TokenKind::Minus:
                case TokenKind::Star:
                case TokenKind::ForSlash:
                case TokenKind::Percent:
                    return left_type; // safe: both are same and numeric

                case TokenKind::EqualEqual:
                case TokenKind::NotEqual:
                case TokenKind::LesserThan:
                case TokenKind::LesserEqual:
                case TokenKind::GreaterThan:
                case TokenKind::GreaterEqual:
                case TokenKind::And:
                case TokenKind::Or:
                    return SmplType::Boolean;

                case TokenKind::As:
                    return right_type; // type cast to right

                case TokenKind::Range:
                    return SmplType::Range;

                default:
                    throw CompilerError("Unknown operator", binary->get_line());
            }
        }
        case ExprASTKind::BooleanLiteral: return SmplType::Boolean;
        case ExprASTKind::FnCall: {
            FuncCallNode* fncall = dynamic_cast<FuncCallNode*>(expr.get());
            Symbol* fndecl = lookup(fncall->identifier.lexeme);
            if (!fndecl) {
                throw CompilerError(std::format("{} is not defined", fncall->identifier.lexeme), fncall->get_line());
            }

            if (fncall->args.size() != fndecl->param_types.size()) {
                throw CompilerError(std::format("Function call '{}' has {} arguments, expected {}",
                                                        fncall->identifier.lexeme, fncall->args.size(), fndecl->param_types.size()),
                                    fncall->get_line());
            }

            auto& param_types = fndecl->param_types;
            auto& args = fncall->args;
            for (int i = 0; i < args.size(); i++) {
                if (!TypeChecker::are_assign_compatible(param_types[i], analyze_expr(args[i]))) {
                    // TODO: Better error message
                    throw CompilerError("Incompatible types", fncall->get_line());
                }
            }

            return fndecl->type;
        }
        case ExprASTKind::Grouping: {
            GroupingExpr* group = dynamic_cast<GroupingExpr*>(expr.get());
            return analyze_expr(group->expr);
        }
        case ExprASTKind::Identifier: {
            IdentifierNode* identifier = dynamic_cast<IdentifierNode*>(expr.get());
            Symbol* variable = lookup(identifier->identifier.lexeme);
            if (!variable) {
                throw CompilerError(std::format("{} is not defined", identifier->identifier.lexeme), identifier->get_line());
            }

            return variable->type;
        }
        case ExprASTKind::NumberLiteral: {
            NumLitNode* numlit = dynamic_cast<NumLitNode*>(expr.get());
            return (numlit->literal.lexeme.contains(".")) ? SmplType::UntypedFloat : SmplType::UntypedInt;
        }
        case ExprASTKind::Unary: {
            UnaryNode* unary = dynamic_cast<UnaryNode*>(expr.get());
            SmplType right_type = analyze_expr(unary->right);
            switch (unary->op.kind) {
                case TokenKind::Not:
                    if (right_type != SmplType::Boolean) {
                        throw TypeError("Logical 'not' operator only accepts a boolean operand", unary->get_line());
                    }
                    return SmplType::Boolean;
                case TokenKind::Minus: {
                    if (!TypeChecker::is_numeric(right_type)) {
                        throw TypeError("Negation '-' operator only accepts numeric values as an operand", unary->get_line());
                    }
                    return right_type;
                }
                default:
                    throw CompilerError("Invalid operator for unary expression", unary->get_line());
            }
        }
    }
}
