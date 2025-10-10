#include "../../include/Optimization/Optimizer.hpp"

namespace Lexer::Opt {
Optimizer::Optimizer(std::vector<AST::Stmt::Ptr>& ast)
    : ast(ast) {
}

void Optimizer::optimize(flag_t flags) {
    // Placeholder for optimization logic
    // For example, you could implement constant folding, dead code elimination, etc.
    for (auto& stmt : ast) {
        // Example: If the statement is an expression statement with a constant expression, we could simplify it
        // This is just a placeholder and does not represent actual optimization logic
        // if (auto exprStmt = dynamic_cast<AST::ExpressionStmt*>(stmt.get())) {
        //     if (auto constExpr = dynamic_cast<AST::Number*>(exprStmt->expression.get())) {
        //         // Perform some optimization on constExpr
        //     }
        // }
    }
}

std::string Optimizer::transpile() const {
    std::ostringstream os;
    std::ostringstream vars;
    for (const auto& stmt : ast) {
        stmt->transpile(os, vars, 4);
    }
    std::stringstream main;
    std::string varStr = vars.str();
    if (!varStr.empty()) {
        varStr = varStr.substr(0, varStr.size() - 2);
    }

    main << "#include \"types/JsAny.hpp\"\n"
        << "#include \"global/global.hpp\"\n"
        << "#include \"types/objects/Types.hpp\"\n"
        << "#include \"customOperators/CustomOperators.hpp\"\n\n"
        << "int main() {\n";
    if (!varStr.empty()) {
        main << "    JS::Any " << varStr << ";\n";
    }
    main << os.str()
        << "    return 0;\n"
        << "}\n";

    return main.str();
}
}