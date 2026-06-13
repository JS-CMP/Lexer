#ifndef JS_CMP_LEXER_NODE_HPP
#define JS_CMP_LEXER_NODE_HPP
#include <memory>
#include <vector>
#include <sstream>
#include <unordered_set>

namespace Lexer::AST {
class Parser;

struct Scope {
    Scope* parent = nullptr;
    std::unordered_set<std::string> vars;
    std::vector<std::string> order;

    bool declare(const std::string& name) {
        if (vars.count(name)) return false;
        vars.insert(name);
        order.push_back(name);
        return true;
    }

    std::string emitVars() const {
        if (order.empty()) return "";
        std::string result;
        for (size_t i = 0; i < order.size(); i++) {
            if (i > 0) result += ", ";
            result += order[i];
        }
        return result;
    }
};

struct TranspileContext {
    Scope* currentScope = nullptr;
};

class Node {
public:
    using Ptr = std::unique_ptr<Node>;

    virtual ~Node() = default;
    virtual void print(size_t indent = 0) const = 0;
    virtual std::ostringstream& transpile(std::ostringstream& os, TranspileContext& ctx, size_t indent = 0) const = 0;
};


class Expr : public Node {
public:
    using Ptr = std::unique_ptr<Expr>;
    static Expr::Ptr parse(Parser& parser);
};


class Stmt : public Node {
public:
    using Ptr = std::unique_ptr<Stmt>;
    static Stmt::Ptr parse(Parser& parser);
};
}

#define CTOR(...) __VA_ARGS__
#define DECL(x, ...) x; DECL2(__VA_ARGS__)
#define DECL2(x, ...) x; DECL3(__VA_ARGS__)
#define DECL3(x, ...) x; DECL4(__VA_ARGS__)
#define DECL4(x, ...) x; DECL5(__VA_ARGS__)
#define DECL5(x, ...) x; DECL6(__VA_ARGS__)
#define DECL6(x, ...) x;

#define EXPR_DEFAULT(x) class x : public Expr {\
public:\
    x() = default;\
    void print(size_t indent) const final;\
    std::ostringstream& transpile(std::ostringstream& os, TranspileContext& ctx, size_t indent) const final;\
    static Expr::Ptr parse(Parser& parser); \
};

#define EXPR(x, ctor, ...) class x : public Expr {\
public: \
    explicit x(__VA_ARGS__) : ctor {};\
    void print(size_t indent) const final;\
    std::ostringstream& transpile(std::ostringstream& os, TranspileContext& ctx, size_t indent) const final;\
    static Expr::Ptr parse(Parser& parser, Expr::Ptr expr = nullptr);\
    DECL(__VA_ARGS__)\
};

#define STMT_DEFAULT(x) class x : public Stmt {\
public:\
    x() = default;\
    void print(size_t indent) const final;\
    std::ostringstream& transpile(std::ostringstream& os, TranspileContext& ctx, size_t indent) const final;\
    static Stmt::Ptr parse(Parser& parser);\
};

#define STMT(x, ctor, ...) class x : public Stmt {\
public: \
    explicit x(__VA_ARGS__) : ctor {};\
    void print(size_t indent) const final;\
    std::ostringstream& transpile(std::ostringstream& os, TranspileContext& ctx, size_t indent) const final;\
    static Stmt::Ptr parse(Parser& parser);\
    DECL(__VA_ARGS__)\
};

#endif //JS_CMP_LEXER_NODE_HPP