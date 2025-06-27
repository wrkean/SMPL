#pragma once

#include "ast/expr/binary.hpp"
#include "ast/expr/boolean_literal.hpp"
#include "ast/expr/cond_expr.hpp"
#include "ast/expr/expr.hpp"
#include "ast/expr/fncall.hpp"
#include "ast/expr/grouping.hpp"
#include "ast/expr/identifier.hpp"
#include "ast/expr/num_literal.hpp"
#include "ast/expr/string_literal.hpp"
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
#include "ast/stmt/stmt.hpp"
#include "ast/stmt/while.hpp"
#include "semantic_analyzer/semantic_analyzer.hpp"
#include <fstream>
#include <memory>
#include <vector>

class CodeGenerator {
public:
    CodeGenerator(std::vector<std::unique_ptr<StmtNode>>& program, SemanticAnalyzer& analyzer);
    void gen();

private:
    std::vector<std::unique_ptr<StmtNode>>& program;
    SemanticAnalyzer& analyzer;
    std::ofstream output_file;
    int indent_level;

    void gen_statement(std::unique_ptr<StmtNode>& stmt);
    std::string gen_expression(std::unique_ptr<ExprNode>& expr);

    void gen_assignment(AssignmentNode* assignment);
    void gen_block(BlockNode* block);
    void gen_expr_stmt(ExprStmt* expr_stmt);
    void gen_fndecl(DefnNode* fndecl);
    void gen_params(ParamNode* param_node);
    void gen_for(ForNode* for_node);
    void gen_if(IfNode* if_node);
    void gen_return(ReturnNode* return_node);
    void gen_while(WhileNode* while_node);

    std::string gen_binary(BinaryExpr* binary_expr);
    std::string gen_boolean(BooleanLiteral* boolean_literal);
    std::string gen_cond_expr(CondExprNode* cond_expr);
    std::string gen_fncall(FuncCallNode* fncall_node);
    std::string gen_grouping(GroupingExpr* grouping);
    std::string gen_identifier(IdentifierNode* identifier);
    std::string gen_numlit(NumLitNode* numlit);
    std::string gen_stringlit(StringLiteral* stringlit);
    std::string gen_type(TypeNode* type_node);
    std::string gen_unary(UnaryNode* unary);

    void indent();
    std::string map_type(const std::string& smpl_type) const;
    std::string map_operator(TokenKind op_kind) const;
};
