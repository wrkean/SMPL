#include "ast/stmt/assignment.hpp"
#include <iostream>
AssignmentNode::AssignmentNode(Token variable, Token var_type, Token op, std::unique_ptr<ExprNode> right)
    : variable(variable), var_type(var_type), op(op), right(std::move(right)) { }

void AssignmentNode::print() const {
    std::cout << "Variable: " << variable.lexeme << "\n";
    std::cout << "Type: " << var_type.lexeme << "\n";
    std::cout << "Operator: " << op.lexeme << "\n";
    std::cout << "Expr: ";
    right->print();
    std::cout << "\n";
}
