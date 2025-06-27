#include "semantic_analyzer/semantic_analyzer.hpp"
#include "ast/ast_kinds.hpp"
#include "ast/expr/binary.hpp"
#include "ast/expr/cond_expr.hpp"
#include "ast/expr/expr.hpp"
#include "ast/expr/fncall.hpp"
#include "ast/expr/grouping.hpp"
#include "ast/expr/identifier.hpp"
#include "ast/expr/num_literal.hpp"
#include "ast/expr/type.hpp"
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
#include "magic_enum.hpp"
#include "smpl/builtins.hpp"
#include "smpl/smpl.hpp"
#include "smpl/symbol/func_symbol.hpp"
#include "smpl/symbol/symbol.hpp"
#include "smpl/symbol/var_symbol.hpp"
#include "smpl/type_checker.hpp"
#include "smpl/types.hpp"
#include "token/tokenkind.hpp"
#include <cstdlib>
#include <format>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// TODO: Finish and clean these up
SemanticAnalyzer::SemanticAnalyzer(std::vector<std::unique_ptr<StmtNode>>& program)
    : program(program), cur_func_return_type(SmplType::Unknown)
{
    enter_scope();
    // Declare builtins
    auto print_str = std::make_unique<FuncSymbol>(SmplType::Void, std::vector{SmplType::String}, Token(TokenKind::Identifier, "print_str", 1));
    declare_symbol("print_str", std::move(print_str));
    auto print_int = std::make_unique<FuncSymbol>(SmplType::Void, std::vector{SmplType::Int}, Token(TokenKind::Identifier, "print_int", 1));
    declare_symbol("print_int", std::move(print_int));
    auto print_float = std::make_unique<FuncSymbol>(SmplType::Void, std::vector{SmplType::Float32}, Token(TokenKind::Identifier, "print_float", 1));
    declare_symbol("print_float", std::move(print_float));
};

bool SemanticAnalyzer::declare_symbol(const std::string& name, std::unique_ptr<Symbol> symbol) {
    if (builtin::is_builtin(name)) return false;
    if (symbol_table.empty()) throw CompilerError("No scope available for declaration", symbol->get_token().line);
    if (symbol_table.back().contains(name)) return false;   // Cant declare, already declared

    try {
        tc::str_to_type(name, symbol->get_token().line);
    } catch (...) {
        symbol_table.back().emplace(name, std::move(symbol));
        return true;
    }

    throw CompilerError(std::format("Cannot declare built-in primitive type name: {}", name), symbol->get_token().line);
}

Symbol* SemanticAnalyzer::lookup(const std::string& name) {
    for (auto it = symbol_table.rbegin(); it != symbol_table.rend(); ++it) {
        if (it->contains(name)) {
            return it->at(name).get();
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

bool SemanticAnalyzer::analyze() {
    bool had_error = false;
    for (auto& statement : program) {
        try {
            analyze_stmt(statement);
        } catch (const CompilerError& err) {
            Smpl::error(err.line_number, err.what());
            had_error = true;
        }
    }
    exit_scope();

    return had_error;
}

void SemanticAnalyzer::analyze_stmt(std::unique_ptr<StmtNode>& stmt) {
    switch (stmt->get_kind()) {
        case StmtASTKind::Assignment: {
            AssignmentNode* assignment = dynamic_cast<AssignmentNode*>(stmt.get());
            SmplType ass_type = tc::str_to_type(assignment->var_type.lexeme, stmt->get_line());
            //       ^^^ as in assignment
            SmplType right_type = analyze_expr(assignment->right);
            // TODO: Better error report
            if (!tc::are_assign_compatible(ass_type, right_type)) {
                std::ostringstream left_type_str;
                std::ostringstream right_type_str;
                left_type_str << magic_enum::enum_name(ass_type);
                right_type_str << magic_enum::enum_name(right_type);
                throw TypeError(std::format("Incompatible types for assignment: {} to {}", right_type_str.str(), left_type_str.str()), stmt->get_line());
            } else {
                if (auto* numlit = dynamic_cast<NumLitNode*>(assignment->right.get())) {
                    if (!tc::fits_in_type(ass_type, numlit->literal.lexeme)) {
                        // TODO: Better error messages
                        throw CompilerError("An overflow will occur here", numlit->get_line());
                    }
                }
            }
            auto symbol = std::make_unique<VarSymbol>(ass_type, assignment->variable);
            if (!declare_symbol(assignment->variable.lexeme, std::move(symbol))) {
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

            try {
            if (defn->return_type.has_value()) {
                return_type = tc::str_to_type(defn->return_type->lexeme, defn->get_line());
            }
            cur_func_return_type = return_type;

            std::vector<SmplType> param_types;
            ParamNode* param_node = dynamic_cast<ParamNode*>(defn->params.get());
            for (auto& param : param_node->params) {
                SmplType param_type = tc::str_to_type(param.second.lexeme, param_node->get_line());
                param_types.push_back(param_type);
            }

            auto symbol = std::make_unique<FuncSymbol>(return_type, param_types, defn->identifier);
            if (!declare_symbol(defn->identifier.lexeme, std::move(symbol))) {
                throw CompilerError(std::format("{} is already defined", defn->identifier.lexeme), defn->get_line());
            }
            } catch (const CompilerError& err) {
                Smpl::error(err.line_number, err.what());
                return;
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
            auto symbol = std::make_unique<VarSymbol>(type, for_node->bind_var);
            declare_symbol(for_node->bind_var.lexeme, std::move(symbol));
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
                SmplType type = tc::str_to_type(param_.second.lexeme, param->get_line());
                auto symbol = std::make_unique<VarSymbol>(type, param_.first);
                declare_symbol(param_.first.lexeme, std::move(symbol));
            }
        } break;
        case StmtASTKind::Return: {
            ReturnNode* return_node = dynamic_cast<ReturnNode*>(stmt.get());
            SmplType return_type = SmplType::Void;
            if (return_node->return_expr != nullptr) {
                return_type = analyze_expr(return_node->return_expr);
            }

            if (!tc::are_assign_compatible(cur_func_return_type, return_type)) {
                if (return_node->return_expr != nullptr)
                    throw TypeError("Expression type does not match the function's return type", return_node->return_expr->get_line());
                throw TypeError("Function expects a return value, returned 'void' instead", return_node->get_line());
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
            std::ostringstream left_type_str, right_type_str, op_str;
            left_type_str << magic_enum::enum_name(left_type);
            right_type_str << magic_enum::enum_name(right_type);

            if (!tc::are_binary_compatible(left_type, right_type)) {
                if (binary->op.kind == TokenKind::As) {
                    // Except for as operator, we go out of the if statement
                    goto escape;
                }
                op_str << magic_enum::enum_name(binary->op.kind);
                throw TypeError(std::format("Incompatible types for binary operation: {} <{}> {}", left_type_str.str(), op_str.str(), right_type_str.str()), binary->get_line());
            }
        escape:
            switch (binary->op.kind) {
                case TokenKind::Plus:
                case TokenKind::Minus:
                case TokenKind::Star:
                case TokenKind::ForSlash:
                case TokenKind::Percent:
                case TokenKind::Equal:
                case TokenKind::PlusEqual:
                case TokenKind::MinusEqual:
                case TokenKind::StarEqual:
                case TokenKind::ForSlashEqual:
                case TokenKind::PercentEqual:
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

                case TokenKind::As: {
                    if (binary->right->get_kind() != ExprASTKind::Type)
                        throw CompilerError("Expected a type for right hand operand for 'as' opeprator", binary->right->get_line());
                    if (tc::is_castable(left_type, right_type)) {
                        return right_type;
                    }
                    throw TypeError(std::format("Invalid cast from {} to {}", left_type_str.str(), right_type_str.str()), binary->op.line);
                }

                case TokenKind::Range:
                    return SmplType::Range;

                default:
                    throw CompilerError("Unknown operator", binary->get_line());
            }
        }
        case ExprASTKind::BooleanLiteral: return SmplType::Boolean;
        case ExprASTKind::FnCall: {
            FuncCallNode* fncall = dynamic_cast<FuncCallNode*>(expr.get());
            Symbol* fndecl = nullptr;
            SmplType arg_type = analyze_expr(fncall->args[0]);
            switch (builtin::get_builtin(fncall->identifier.lexeme)) {
                case builtin::PrintFunc: {
                    if (tc::is_integer(arg_type)) {
                        fndecl = lookup("print_int");
                    } else if (tc::is_floating(arg_type)) {
                        fndecl = lookup("print_float");
                    } else if (arg_type == SmplType::String) {
                        fndecl = lookup("print_str");
                    } else {
                        throw TypeError("Built-in 'print' function only accepts integer, float, and string expressions", fncall->get_line());
                    }
                    // Skip type checking
                    auto fn_symbol = dynamic_cast<FuncSymbol*>(fndecl);
                    if (fncall->args.size() != fn_symbol->param_types.size()) {
                        throw CompilerError(std::format("Function call '{}' has {} arguments, expected {}", fncall->identifier.lexeme, fncall->args.size(), fn_symbol->param_types.size()), fncall->get_line());
                    }
                    return SmplType::Void;
                } break;
                case builtin::None: {
                    fndecl = lookup(fncall->identifier.lexeme);
                } break;
            }
            if (!fndecl) {
                throw CompilerError(std::format("{} is not defined", fncall->identifier.lexeme), fncall->get_line());
            }

            auto fn_symbol = dynamic_cast<FuncSymbol*>(fndecl);
            if (fncall->args.size() != fn_symbol->param_types.size()) {
                throw CompilerError(std::format("Function call '{}' has {} arguments, expected {}",
                                                        fncall->identifier.lexeme, fncall->args.size(), fn_symbol->param_types.size()),
                                    fncall->get_line());
            }

            auto& param_types = fn_symbol->param_types;
            auto& args = fncall->args;
            for (int i = 0; i < args.size(); i++) {
                if (!tc::are_assign_compatible(param_types[i], analyze_expr(args[i]))) {
                    // TODO: Better error message
                    throw CompilerError("Incompatible types", fncall->get_line());
                }
            }

            return fn_symbol->return_type;
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

            auto var_symbol = dynamic_cast<VarSymbol*>(variable);
            return var_symbol->type;
        }
        case ExprASTKind::Type: {
            TypeNode* type_node = dynamic_cast<TypeNode*>(expr.get());
            return tc::str_to_type(type_node->type.lexeme, type_node->get_line());
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
                    if (!tc::is_numeric(right_type)) {
                        throw TypeError("Negation '-' operator only accepts numeric values as an operand", unary->get_line());
                    }
                    return right_type;
                }
                default:
                    throw CompilerError("Invalid operator for unary expression", unary->get_line());
            }
        }
        case ExprASTKind::CondExpr: {
            CondExprNode* cond_expr = dynamic_cast<CondExprNode*>(expr.get());

            auto if_val_type = analyze_expr(cond_expr->if_val);
            auto if_expr_type = analyze_expr(cond_expr->if_expr);
            auto else_val_type = analyze_expr(cond_expr->else_val);

            if (if_expr_type != SmplType::Boolean)
                throw TypeError("Condition after if must be a boolean expression", cond_expr->if_expr->get_line());
            if (if_val_type != else_val_type)
                throw TypeError("Default value and the else value must be the same type", cond_expr->get_line());

            return if_val_type;
        }
        case ExprASTKind::StringLiteral: return SmplType::String;
    }
}
