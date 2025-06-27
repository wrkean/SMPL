#include "ast/expr/cond_expr.hpp"
#include "ast/ast_kinds.hpp"
#include "smpl/types.hpp"
#include <utility>

CondExprNode::CondExprNode(std::unique_ptr<ExprNode> if_val,
                           std::unique_ptr<ExprNode> if_expr,
                           std::unique_ptr<ExprNode> else_val,
                           size_t line)
    : if_val(std::move(if_val)),
    if_expr(std::move(if_expr)),
    else_val(std::move(else_val)),
    line(line),
    kind(ExprASTKind::CondExpr),
    type(SmplType::Unknown) { }

void CondExprNode::print() const {

}
