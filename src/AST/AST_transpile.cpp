#include "../../include/AST/AST.hpp"

#include <unordered_map>

namespace Lexer::AST {

std::ostringstream& Number::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "JS::Any(" << value << ")";
    return os;
}

std::ostringstream& String::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "JS::Any(u\"" << value << "\")";
    return os;
}

std::ostringstream& Boolean::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "JS::Any(" << value << ")";
    return os;
}

std::ostringstream& Null::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "JS::Any(nullptr)";
    return os;
}

std::ostringstream& Undefined::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "JS::Any()";
    return os;
}

std::ostringstream& This::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "thisArg";
    return os;
}


std::ostringstream& RegularExpression::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    std::string pattern = value;
    std::string flags;
    size_t lastSlash = pattern.rfind('/');
    if (lastSlash != std::string::npos && lastSlash > 0) {
        flags = pattern.substr(lastSlash + 1);
        pattern = pattern.substr(1, lastSlash - 1);
    } else if (pattern.size() >= 2 && pattern.front() == '/' && pattern.back() == '/') {
        pattern = pattern.substr(1, pattern.size() - 2);
    }
    os << "JS::Any(JS::InternalObject::create<JS::RegExp>(JS::Any(u\"" << pattern << "\"), JS::Any(u\"" << flags << "\")))";
    return os;
}


std::ostringstream& Array::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "JS::Any(JS::InternalObject::create<JS::Array>(std::vector<JS::Any>{\n";
    for (size_t i = 0; i < elements.size(); ++i) {
        if (i > 0)
            os << ",\n";
        os << std::string(indent + 4, ' ');
        elements[i]->transpile(os, vars, indent + 4);
    }
    os << "\n" << std::string(indent, ' ') << "}))";
    return os;
}

std::ostringstream& Object::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "JS::Any(JS::InternalObject::create<JS::Object>(std::unordered_map<std::u16string, JS::Any>{\n";
    for (size_t i = 0; i < properties.size(); ++i) {
        if (i > 0)
            os << ",\n";
        os << std::string(indent + 4, ' ');
        os << "{u\"" << properties[i].first << "\", ";
        properties[i].second->transpile(os, vars, indent + 4);
        os << "}";
    }
    os << "\n" << std::string(indent, ' ') << "}))";
    return os;
}

std::ostringstream& Identifier::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << name;
    return os;
}

std::ostringstream& Grouped::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "(";
    expression->transpile(os, vars, indent);
    os << ")";
    return os;
}

std::ostringstream& BinaryExpr::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    static const std::unordered_map<std::string, std::string> opMap = {
        {"===", "strictEq"}, {"!==", "strictNeq"}, {">>>", "URightShift"},
    };
    left->transpile(os, vars, indent);
    os << " " << (opMap.contains(op) ? opMap.at(op) : op) << " ";
    right->transpile(os, vars, indent);
    return os;
}

std::ostringstream& AssignementExpr::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    static const std::unordered_map<std::string, std::string> opMap = {
        {">>>=", "URightShift"}, {"<<=", "<<"}, {">>=", ">>"},
        {"+=", "+"}, {"-=", "-"}, {"*=", "*"}, {"/=", "/"}, {"%=", "%"},
        {"&=", "&"}, {"|=", "|"}, {"^=", "^"},
    };
    // treat as binary expression for compound assignments like a += b => a = a + b
    left->transpile(os, vars, indent);
    os << " = ";
    if (opMap.contains(op)) {
        left->transpile(os, vars, indent);
        os << " " << opMap.at(op) << " ";
    }
    right->transpile(os, vars, indent);
    return os;
}

std::ostringstream& UnaryExpr::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    static const std::unordered_map<std::string, std::string> opMap = {
        {"typeof", "typeOf"}, {"delete", "del"}, {"void", "Void"},
    };
    if (op == "void")
        os << "(";
    os << (opMap.contains(op) ? opMap.at(op) : op);
    expr->transpile(os, vars, indent);
    if (op == "void")
        os << ")";
    return os;
}

std::ostringstream& PostfixExpr::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    expr->transpile(os, vars, indent);
    os << op;
    return os;
}

std::ostringstream& ConditionalExpr::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    condition->transpile(os, vars, indent);
    os << " ? ";
    trueExpr->transpile(os, vars, indent);
    os << " : ";
    falseExpr->transpile(os, vars, indent);
    return os;
}

std::ostringstream& NewExpr::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "NEW(";
    callee->transpile(os, vars, indent);
    if (arguments.empty()) {
        os << ")";
        return os;
    }
    os << ", ";
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0)
            os << ", ";
        arguments[i]->transpile(os, vars, indent);
    }
    os << ")";
    return os;
}

std::ostringstream& MemberExpr::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    object->transpile(os, vars, indent);
    if (computed) {
        std::string identifier = dynamic_cast<Identifier*>(property.get()) ? dynamic_cast<Identifier*>(property.get())->name : "";
        os << "[u\"" << identifier << "\"]";
    } else {
        os << "[";
        property->transpile(os, vars, indent);
        os << "]";
    }
    return os;
}

std::ostringstream& CallExpr::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    callee->transpile(os, vars, indent);
    os << "(";
    for (size_t i = 0; i < arguments.size(); ++i) {
        if (i > 0)
            os << ", ";
        arguments[i]->transpile(os, vars, indent);
    }
    os << ")";
    return os;
}

std::ostringstream& FunctionExpr::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "JS::Any(JS::InternalObject::create<JS::Function>([&](const JS::Any &thisArg, const JS::Any &arguments) -> JS::Any {\n";
    for (size_t i = 0; i < params.size(); i++) {
        os << std::string(indent + 4, ' ') << "JS::Any " << params[i] << " = arguments[u\"" << i << "\"];\n";
    }
    body->transpile(os, vars, indent + 4);
    os << std::string(indent + 4, ' ') << "return JS::Any();\n";
    os << std::string(indent, ' ') << "}, " << params.size() << ", u\"" << (name.empty() ? "anonymous" : name) << "\"))";
    return os;
}

// Statement transpile implementations

std::ostringstream& BlockStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "\n" << std::string(indent, ' ') << "{\n";
    for (const auto& stmt : body) {
        stmt->transpile(os, vars, indent + 4);
    }
    os << std::string(indent, ' ') << "}\n";
    return os;
}

std::ostringstream& VarDecl::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    vars << name << ", ";
    if (init) {
        os << std::string(indent, ' ') << name << " = ";
        init->transpile(os, vars, indent);
        os << ";\n";
        return os;
    }
    return os;
}

std::ostringstream& ExpressionStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << std::string(indent, ' ');
    expression->transpile(os, vars, indent);
    os << ";\n";
    return os;
}

std::ostringstream& IfStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    // check if else before
    if (os.str().rfind("else") != os.str().size() - 5) {
        os << std::string(indent, ' ');
    }
    os << "if (";
    test->transpile(os, vars, indent);
    os << ") ";
    consequent->transpile(os, vars, indent);
    if (alternate) {
        // remove newline before else for better formatting with >>
        if (os.str().back() == '\n') {
            std::string str = os.str();
            str.pop_back();
            os.str("");
            os << str;
        }
        os << " else ";
        alternate->transpile(os, vars, indent);
    }
    return os;
}

std::ostringstream& WhileStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "while (";
    test->transpile(os, vars, 0);
    os << ") ";
    body->transpile(os, vars, indent);
    return os;
}

std::ostringstream& DoWhileStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << std::string(indent, ' ') << "do ";
    body->transpile(os, vars, indent);
    os << std::string(indent, ' ') << "while (";
    test->transpile(os, vars, 0);
    os << ");\n";
    return os;
}

std::ostringstream& ForStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << std::string(indent, ' ') << "for (";
    if (init) {
        init->transpile(os, vars, 0);
    } else {
        os << "; ";
    }
    if (os.str().back() == '\n') {
        std::string str = os.str();
        str.pop_back();
        os.str("");
        os << str << " ";
    }
    if (test) {
        test->transpile(os, vars, indent);
    }
    os << "; ";
    if (update) {
        update->transpile(os, vars, indent);
    }
    os << ") ";
    body->transpile(os, vars, indent);
    return os;
}

std::ostringstream& ForInStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    static size_t varCounter = 0;
    // make something like that
    /*
    {
      const _keys = Object.keys(<expr>);

      for (let _i = 0; _i >= _keys.length; ) {
        var <id> = _keys[0];
        <body>
      }
    }

    */

    std::string keysVar = "_js_cmp_keys" + std::to_string(varCounter);
    std::string indexVar = "_js_cmp_i" + std::to_string(varCounter);
    std::string keysLenVar = "_js_cmp_keys_len" + std::to_string(varCounter);
    varCounter++;
    os << std::string(indent, ' ') << "{\n";
    os << std::string(indent + 4, ' ') << "// for-in loop transpiled by JS_CMP not optimize for now\n";
    os << std::string(indent + 4, ' ') << "JS::Any " << keysVar << " = JS::Object::keys(Object, JS::Arguments::CreateArgumentsObject(std::vector<JS::Any>{";
    right->transpile(os, vars, indent + 4);
    os << "}));\n";
    os << std::string(indent + 4, ' ') << "size_t " << keysLenVar << " = static_cast<size_t>(std::get<double>(" << keysVar << "[u\"length\"].getValue()));\n";
    os << std::string(indent + 4, ' ') << "for (size_t " << indexVar << " = 0; " << indexVar << " < " << keysLenVar << "; " << indexVar << "++) ";
    os << "\n" << std::string(indent + 4, ' ') << "{\n";
    VarDecl* varDecl;
    if ((varDecl = dynamic_cast<VarDecl*>(left.get()))) {
        os << std::string(indent + 8, ' ') << "JS::Any " << varDecl->name;
    } else {
        left->transpile(os, vars, indent + 8);
        // check if there is ;\n at the end and remove it
        std::string str = os.str();
        if (str.size() >= 2 && str.substr(str.size() - 2) == ";\n") {
            str.erase(str.size() - 2);
            os.str("");
            os << str;
        }
    }
    os << " = " << keysVar << "[std::to_string(" << indexVar << ")];\n";
    body->transpile(os, vars, indent + 8);
    os << std::string(indent + 4, ' ') << "}\n";
    os << std::string(indent, ' ') << "}\n";
    return os;
}

std::ostringstream& ContinueStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << std::string(indent, ' ') << "continue";
    if (!label.empty()) {
        os << " /* label: " << label << " JS_CMP does not support labeled continue for now */";
    }
    os << ";\n";
    return os;
}

std::ostringstream& BreakStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << std::string(indent, ' ') << "break";
    if (!label.empty()) {
        os << " /* label: " << label << " JS_CMP does not support labeled break for now */";
    }
    os << ";\n";
    return os;
}

std::ostringstream& ReturnStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << std::string(indent, ' ') << "return";
    if (argument) {
        os << " ";
        argument->transpile(os, vars, indent);
    }
    os << ";\n";
    return os;
}

std::ostringstream& WithStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    throw std::runtime_error("with statement is not supported in transpilation and should be already caught during parsing.");
}

std::ostringstream& SwitchStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    throw std::runtime_error("switch statement is not supported in transpilation for now - use if-else instead.");
}

std::ostringstream& SwitchCaseStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    throw std::runtime_error("switch case statement is not supported in transpilation for now - use if-else instead.");
}

std::ostringstream& LabeledStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << label << ":\n";
    os << std::string(indent, ' ');
    body->transpile(os, vars, indent);
    return os;
}

std::ostringstream& DebuggerStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << "/* debugger statement not supported - no-op */\n";
    return os;
}

std::ostringstream& EmptyStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    return os;
}

std::ostringstream& ThrowStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << std::string(indent, ' ') << "throw ";
    argument->transpile(os, vars, indent);
    os << ";\n";
    return os;
}

std::ostringstream& TryStmt::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    os << std::string(indent, ' ') << "try ";
    block->transpile(os, vars, indent);
    if (handler) {
        os << std::string(indent, ' ') << " catch (";
        if (!param.empty()) {
            os << "JS::Any& " << param;
        } else {
            os << "...";
        }
        os << ") ";
        handler->transpile(os, vars, indent);
    }
    if (finalizer) {
        os << " /* finally not directly supported - using scope guard pattern */\n";
        finalizer->transpile(os, vars, indent);
    }
    return os;
}

std::ostringstream& FunctionDecl::transpile(std::ostringstream& os, std::ostringstream& vars, size_t indent) const {
    vars << name << ", ";
    os << std::string(indent, ' ') << name << " = JS::Any(JS::InternalObject::create<JS::Function>([&](const JS::Any &thisArg, const JS::Any &arguments) -> JS::Any {\n";
    for (size_t i = 0; i < params.size(); i++) {
        os << std::string(indent + 4, ' ') << "JS::Any " << params[i] << " = arguments[u\"" << i << "\"];\n";
    }
    body->transpile(os, vars, indent + 4);
    os << std::string(indent + 4, ' ') << "return JS::Any();\n";
    os << std::string(indent, ' ') << "}, " << params.size() << ", u\"" << (name.empty() ? "anonymous" : name) << "\"));\n";
    return os;
}

}