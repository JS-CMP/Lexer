#include "../../include/AST/AST.hpp"

#include <iostream>

namespace Lexer::AST {
void Number::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "Number(" << value << ")\n";
}

void String::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "String(\"" << value << "\")\n";
}

void Boolean::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "Boolean(" << value << ")\n";
}

void RegularExpression::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "RegularExpression(" << value << ")\n";
}

void Array::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "ArrayExpr\n";
    for (const auto& element : elements) {
        element->print(indent + 2);
    }
}


void Object::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "ObjectExpr\n";
    for (const auto& prop : properties) {
        std::cout << std::string(indent + 2, ' ') << "Property: " << prop.first << "\n";
        prop.second->print(indent + 4);
    }
}

void Null::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "Null\n";
}

void Undefined::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "Undefined\n";
}

void This::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "This\n";
}

void Identifier::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "Identifier(" << name << ")\n";
}

void Grouped::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "GroupedExpr\n";
    expression->print(indent + 2);
}

void AssignementExpr::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "AssignementExpr(" << op << ")\n";
    left->print(indent + 2);
    right->print(indent + 2);
}

void PostfixExpr::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "PostfixExpr(" << op << ")\n";
    expr->print(indent + 2);
}

void BinaryExpr::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "BinaryExpr(" << op << ")\n";
    left->print(indent + 2);
    right->print(indent + 2);
}

void UnaryExpr::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "UnaryExpr(" << op << ")\n";
    expr->print(indent + 2);
}

void ConditionalExpr::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "ConditionalExpr\n";
    condition->print(indent + 2);
    trueExpr->print(indent + 2);
    falseExpr->print(indent + 2);
}

void NewExpr::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "NewExpr\n";
    callee->print(indent + 2);
    std::cout << std::string(indent + 2, ' ') << "Arguments:\n";
    for (const auto& arg : arguments) {
        arg->print(indent + 4);
    }
}

void MemberExpr::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "MemberExpr(" << (computed ? "computed" : "non-computed") << ")\n";
    object->print(indent + 2);
    property->print(indent + 2);
}

void CallExpr::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "CallExpr\n";
    callee->print(indent + 2);
    std::cout << std::string(indent + 2, ' ') << "Arguments:\n";
    for (const auto& arg : arguments) {
        arg->print(indent + 4);
    }
}

void BlockStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "BlockStmt\n";
    for (const auto& stmt : body) {
        stmt->print(indent + 2);
    }
    std::cout << std::string(indent, ' ') << "EndBlockStmt\n";
}

void VarDecl::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "VarDecl(" << name << ")\n";
    if (init) {
        init->print(indent + 2);
    } else {
        std::cout << std::string(indent + 2, ' ') << "null\n";
    }
}

void ExpressionStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "ExpressionStmt\n";
    expression->print(indent + 2);
}

void IfStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "IfStmt\n";
    test->print(indent + 2);
    std::cout << std::string(indent, ' ') << "Then\n";
    consequent->print(indent + 2);
    if (alternate) {
        std::cout << std::string(indent, ' ') << "Else\n";
        alternate->print(indent + 2);
    }
    std::cout << std::string(indent, ' ') << "EndIfStmt\n";
}

void WhileStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "WhileStmt\n";
    test->print(indent + 2);
    std::cout << std::string(indent, ' ') << "Do\n";
    body->print(indent + 2);
    std::cout << std::string(indent, ' ') << "EndWhileStmt\n";
}

void DoWhileStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "DoWhileStmt\n";
    body->print(indent + 2);
    std::cout << std::string(indent, ' ') << "While\n";
    test->print(indent + 2);
    std::cout << std::string(indent, ' ') << "EndDoWhileStmt\n";
}

void ForStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "ForStmt\n";
    if (init) {
        init->print(indent + 2);
    } else {
        std::cout << std::string(indent + 2, ' ') << "null\n";
    }
    if (test) {
        test->print(indent + 2);
    } else {
        std::cout << std::string(indent + 2, ' ') << "null\n";
    }
    if (update) {
        update->print(indent + 2);
    } else {
        std::cout << std::string(indent + 2, ' ') << "null\n";
    }
    std::cout << std::string(indent, ' ') << "Do\n";
    body->print(indent + 2);
    std::cout << std::string(indent, ' ') << "EndForStmt\n";
}

void ForInStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "ForInStmt\n";
    left->print(indent + 2);
    right->print(indent + 2);
    std::cout << std::string(indent, ' ') << "Do\n";
    body->print(indent + 2);
    std::cout << std::string(indent, ' ') << "EndForInStmt\n";
}

void ContinueStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "ContinueStmt";
    if (!label.empty()) {
        std::cout << "(" << label << ")";
    }
    std::cout << "\n";
}

void BreakStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "BreakStmt";
    if (!label.empty()) {
        std::cout << "(" << label << ")";
    }
    std::cout << "\n";
}

void ReturnStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "ReturnStmt\n";
    if (argument) {
        argument->print(indent + 2);
    } else {
        std::cout << std::string(indent + 2, ' ') << "null\n";
    }
}

void WithStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "WithStmt\n";
    object->print(indent + 2);
    body->print(indent + 2);
}

void SwitchStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "SwitchStmt\n";
    discriminant->print(indent + 2);
    for (const auto& caseStmt : cases) {
        caseStmt->print(indent + 2);
    }
}

void SwitchCaseStmt::print(size_t indent) const {
    if (test) {
        std::cout << std::string(indent, ' ') << "SwitchCaseStmt\n";
        test->print(indent + 2);
    } else {
        std::cout << std::string(indent, ' ') << "DefaultCaseStmt\n";
    }
    for (const auto& stmt : consequent) {
        stmt->print(indent + 2);
    }
    std::cout << std::string(indent, ' ') << "EndSwitchCaseStmt\n";
}

void LabeledStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "LabeledStmt(" << label << ")\n";
    body->print(indent + 2);
}

void ThrowStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "ThrowStmt\n";
    argument->print(indent + 2);
}

void TryStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "TryStmt\n";
    block->print(indent + 2);
    if (!param.empty()) {
        std::cout << std::string(indent + 2, ' ') << "CatchParam(" << param << ")\n";
    }
    if (handler) {
        handler->print(indent + 2);
    } else {
        std::cout << std::string(indent + 2, ' ') << "null\n";
    }
    if (finalizer) {
        finalizer->print(indent + 2);
    } else {
        std::cout << std::string(indent + 2, ' ') << "null\n";
    }
}

void DebuggerStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "DebuggerStmt\n";
}

void EmptyStmt::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "EmptyStmt\n";
}

void FunctionDecl::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "FunctionDecl(" << name << ")\n";
    std::cout << std::string(indent + 2, ' ') << "Params:\n";
    for (const auto& param : params) {
        std::cout << std::string(indent + 4, ' ') << param << "\n";
    }
    std::cout << std::string(indent + 2, ' ') << "Body:\n";
    body->print(indent + 4);
}

void FunctionExpr::print(size_t indent) const {
    std::cout << std::string(indent, ' ') << "FunctionExpr";
    if (!name.empty()) {
        std::cout << "(" << name << ")";
    }
    std::cout << "\n";
    std::cout << std::string(indent + 2, ' ') << "Params:\n";
    for (const auto& param : params) {
        std::cout << std::string(indent + 4, ' ') << param << "\n";
    }
    std::cout << std::string(indent + 2, ' ') << "Body:\n";
    body->print(indent + 4);
}

}