#include "ast/stmt/assignment.hpp"
#include <iostream>
AssignmentNode::AssignmentNode(Token variable, Token var_type, std::unique_ptr<ExprNode> right, size_t line)
    : variable(variable), var_type(var_type), right(std::move(right)), kind(StmtASTKind::Assignment), line(line) { }

void AssignmentNode::print() const {
    std::cout << "Assignment\n";
    std::cout << "Variable: " << variable.lexeme << "\n";
    std::cout << "Type: " << var_type.lexeme << "\n";
    std::cout << "Expr: ";
    right->print();
    std::cout << "\nEnd assignment for " << variable.lexeme;
}
