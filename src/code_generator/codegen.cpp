#include "code_generator/codegen.hpp"
#include "ast/ast_kinds.hpp"
#include "ast/expr/binary.hpp"
#include "ast/expr/boolean_literal.hpp"
#include "ast/expr/cond_expr.hpp"
#include "ast/expr/fncall.hpp"
#include "ast/expr/grouping.hpp"
#include "ast/expr/identifier.hpp"
#include "ast/expr/num_literal.hpp"
#include "ast/expr/type.hpp"
#include "ast/expr/unary.hpp"
#include "ast/stmt/assignment.hpp"
#include "ast/stmt/block.hpp"
#include "ast/stmt/fndecl.hpp"
#include "ast/stmt/for.hpp"
#include "ast/stmt/param.hpp"
#include "semantic_analyzer/semantic_analyzer.hpp"
#include "smpl/builtins.hpp"
#include "smpl/types.hpp"
#include "token/tokenkind.hpp"
#include <format>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

CodeGenerator::CodeGenerator(std::vector<std::unique_ptr<StmtNode>>& program, SemanticAnalyzer& analyzer)
    : program(program), analyzer(analyzer), output_file("../c_code/generated.c"), indent_level(0) { }

void CodeGenerator::gen() {
    // Include important headers
    output_file << "#include <stdio.h>\n";
    output_file << "#include <stdbool.h>\n";
    output_file << "#include <stdint.h>\n\n";
    for (auto& statement: program) {
        gen_statement(statement);
    }
    output_file.close();
}

void CodeGenerator::gen_statement(std::unique_ptr<StmtNode>& stmt) {
    switch (stmt->get_kind()) {
        case StmtASTKind::Assignment:
            gen_assignment(dynamic_cast<AssignmentNode*>(stmt.get()));
            break;
        case StmtASTKind::Block:
            gen_block(dynamic_cast<BlockNode*>(stmt.get()));
            break;
        case StmtASTKind::ExprStmt:
            gen_expr_stmt(dynamic_cast<ExprStmt*>(stmt.get()));
            break;
        case StmtASTKind::FnDecl:
            gen_fndecl(dynamic_cast<DefnNode*>(stmt.get()));
            break;
        case StmtASTKind::For:
            gen_for(dynamic_cast<ForNode*>(stmt.get()));
            break;
        case StmtASTKind::If:
            gen_if(dynamic_cast<IfNode*>(stmt.get()));
            break;
        case StmtASTKind::Param:
            gen_params(dynamic_cast<ParamNode*>(stmt.get()));
            break;
        case StmtASTKind::Return:
            gen_return(dynamic_cast<ReturnNode*>(stmt.get()));
            break;
        case StmtASTKind::While:
            gen_while(dynamic_cast<WhileNode*>(stmt.get()));
            break;
    }
}

void CodeGenerator::gen_assignment(AssignmentNode* assignment) {
    indent();
    std::string ass_str = std::format(
        "{} {} = {};\n",
        map_type(assignment->var_type.lexeme),
        assignment->variable.lexeme,
        gen_expression(assignment->right)
    );

    output_file << ass_str;
}

void CodeGenerator::gen_block(BlockNode* block) {
    output_file << "{\n";
    indent_level++;
    for (auto& stmt : block->statements) {
        gen_statement(stmt);
    }
    indent_level--;
    indent();
    output_file << "}\n";
}

void CodeGenerator::gen_expr_stmt(ExprStmt* expr_stmt) {
    indent();
    output_file << gen_expression(expr_stmt->expr) << ";\n";
}

void CodeGenerator::gen_fndecl(DefnNode* fndecl) {
    indent();

    std::string type;
    if (fndecl->identifier.lexeme == "main") {
        type = "int";
    } else {
        if (fndecl->return_type.has_value()) {
            type = map_type(fndecl->return_type.value().lexeme);
        } else {
            type = "void";
        }
    }

    std::string fndecl_str = std::format(
        "{} {}",
        type,
        fndecl->identifier.lexeme
    );

    output_file << fndecl_str;
    gen_params(dynamic_cast<ParamNode*>(fndecl->params.get()));

    output_file << " ";
    gen_block(dynamic_cast<BlockNode*>(fndecl->block.get()));
}

void CodeGenerator::gen_params(ParamNode* param_node) {
    output_file << "(";

    auto& params = param_node->params;
    for (auto it = params.begin(); it != params.end(); it++) {
        output_file << map_type(it->second.lexeme) << " " << it->first.lexeme;

        if (it + 1 != params.end()) {
            output_file << ", ";
        }
    }
    output_file << ")";
}

void CodeGenerator::gen_for(ForNode* for_node) {
    // NOTE: Not expandable as it only works on range expressions,
    // modify later.
    indent();
    auto range_op = dynamic_cast<BinaryExpr*>(for_node->iterator.get());
    std::string for_str = std::format(
        "for (int {} = {}; {} < {}; {}++) ",
        for_node->bind_var.lexeme,
        gen_expression(range_op->left),
        for_node->bind_var.lexeme,
        gen_expression(range_op->right),
        for_node->bind_var.lexeme
    );
    output_file << for_str;
    gen_block(dynamic_cast<BlockNode*>(for_node->block.get()));
}

void CodeGenerator::gen_if(IfNode* if_node) {
    for (auto it = if_node->branches.begin(); it != if_node->branches.end(); it++) {
        auto& branch = *it;
        auto block = dynamic_cast<BlockNode*>(branch.block.get());

        if (branch.condition == nullptr) {
            indent();
            output_file << "else ";
            gen_block(block);
        } else {
            if (it == if_node->branches.begin()) {
                indent();
                output_file << "if (" << gen_expression(branch.condition) << ") ";
            } else {
                output_file << "else if (" << gen_expression(branch.condition) << ") ";
            }
            gen_block(block);
        }
    }
}

void CodeGenerator::gen_return(ReturnNode* return_node) {
    indent();
    output_file << "return " << gen_expression(return_node->return_expr) << ";\n";
}

void CodeGenerator::gen_while(WhileNode* while_node) {
    indent();
    output_file << "while (" << gen_expression(while_node->condition) << ") ";
    gen_block(dynamic_cast<BlockNode*>(while_node->block.get()));
}

std::string CodeGenerator::gen_expression(std::unique_ptr<ExprNode>& expr) {
    switch (expr->get_kind()) {
        case ExprASTKind::Binary:
            return gen_binary(dynamic_cast<BinaryExpr*>(expr.get()));
        case ExprASTKind::BooleanLiteral:
            return gen_boolean(dynamic_cast<BooleanLiteral*>(expr.get()));
        case ExprASTKind::CondExpr:
            return gen_cond_expr(dynamic_cast<CondExprNode*>(expr.get()));
        case ExprASTKind::FnCall:
            return gen_fncall(dynamic_cast<FuncCallNode*>(expr.get()));
        case ExprASTKind::Grouping:
            return gen_grouping(dynamic_cast<GroupingExpr*>(expr.get()));
        case ExprASTKind::Identifier:
            return gen_identifier(dynamic_cast<IdentifierNode*>(expr.get()));
        case ExprASTKind::NumberLiteral:
            return gen_numlit(dynamic_cast<NumLitNode*>(expr.get()));
        case ExprASTKind::StringLiteral:
            return gen_stringlit(dynamic_cast<StringLiteral*>(expr.get()));
        case ExprASTKind::Type:
            return gen_type(dynamic_cast<TypeNode*>(expr.get()));
        case ExprASTKind::Unary:
            return gen_unary(dynamic_cast<UnaryNode*>(expr.get()));
    }
}

std::string CodeGenerator::gen_binary(BinaryExpr* binary_expr) {
    std::string op = binary_expr->op.lexeme;
    // if (binary_expr->op.kind == TokenKind::And) {
    //     op = map_operator(TokenKind::And);
    // } else if (binary_expr->op.kind == TokenKind::Or) {
    //     op = map_operator(TokenKind::Or);
    // }
    switch (binary_expr->op.kind) {
        case TokenKind::And: {
            op = map_operator(TokenKind::And);
        } break;
        case TokenKind::Or: {
            op = map_operator(TokenKind::Or);
        } break;
        case TokenKind::As: {
            auto right = dynamic_cast<TypeNode*>(binary_expr->right.get());
            return std::format("({}) {}", map_type(right->type.lexeme), gen_expression(binary_expr->left));
        }
        default:
    }

    std::string expr = std::format("{} {} {}", gen_expression(binary_expr->left), op, gen_expression(binary_expr->right));

    return expr;
}

std::string CodeGenerator::gen_boolean(BooleanLiteral* boolean_literal) {
    return boolean_literal->literal.lexeme;
}

std::string CodeGenerator::gen_cond_expr(CondExprNode* cond_expr) {
    std::string expr = std::format("{} ? {} : {}", gen_expression(cond_expr->if_expr), gen_expression(cond_expr->if_val), gen_expression(cond_expr->else_val));

    return expr;
}

std::string CodeGenerator::gen_fncall(FuncCallNode* fncall_node) {
    std::string result;
    switch (builtin::get_builtin(fncall_node->identifier.lexeme)) {
        case builtin::PrintFunc: {
            auto& caller = analyzer.builtin_calls.at(fncall_node->identifier.lexeme);
            auto& arg_type = caller.at(builtin::PrintFunc)[0];
            switch (arg_type) {
                case SmplType::Int:
                    result = std::format("printf(\"%d\", {})", gen_expression(fncall_node->args[0]));
                    break;
                case SmplType::Float32:
                    result = std::format("printf(\"%f\", {})", gen_expression(fncall_node->args[0]));
                    break;
                case SmplType::String:
                    result = std::format("printf(\"%s\", \"{}\")", gen_expression(fncall_node->args[0]));
                    break;
                default:
            }
        } break;
        case builtin::None: {
            result = std::format("{} (", fncall_node->identifier.lexeme);
            for (auto it = fncall_node->args.begin(); it != fncall_node->args.end(); it++) {
                result.append(gen_expression(*it));

                if (it + 1 != fncall_node->args.end()) {
                    result.append(", ");
                }
            }
            result.append(")");
        } break;
     }
    return result;
}

std::string CodeGenerator::gen_grouping(GroupingExpr* grouping) {
    return std::format("({})", gen_expression(grouping->expr));
}

std::string CodeGenerator::gen_identifier(IdentifierNode* identifier) {
    return identifier->identifier.lexeme;
}

std::string CodeGenerator::gen_numlit(NumLitNode* numlit) {
    return numlit->literal.lexeme;
}

std::string CodeGenerator::gen_stringlit(StringLiteral* stringlit) {
    return stringlit->literal.lexeme;
}

std::string CodeGenerator::gen_type(TypeNode* type_node) {
    return type_node->type.lexeme;
}

std::string CodeGenerator::gen_unary(UnaryNode* unary) {
    std::string op = unary->op.lexeme;
    if (unary->op.kind == TokenKind::Not) {
        op = "!";
    }

    return std::format("{}{}", op, gen_expression(unary->right));
}

void CodeGenerator::indent() {
    for (int i = 0; i < indent_level; i++) {
        output_file << "    ";
    }
}

std::string CodeGenerator::map_type(const std::string& smpl_type) const {
    std::unordered_map<std::string, std::string> c_types = {
        {"i8", "int8_t"},
        {"i16", "int16_t"},
        {"i32", "int32_t"},
        {"i64", "int64_t"},
        {"int", "int"},
        {"u8", "uint8_t"},
        {"u16", "uint16_t"},
        {"u32", "uint32_t"},
        {"u64", "uint64_t"},
        {"uint", "unsigned int"},
        {"f32", "float"},
        {"f64", "double"},
        {"bool", "bool"},
    };

    return c_types.at(smpl_type);
}

std::string CodeGenerator::map_operator(TokenKind op_kind) const {
    std::unordered_map<TokenKind, std::string> c_operators = {
        {TokenKind::Not, "!"},
        {TokenKind::And, "&&"},
        {TokenKind::Or, "||"},
    };

    return c_operators.at(op_kind);
}
