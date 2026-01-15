#ifndef JS_CMP_LEXER_AST_HPP
#define JS_CMP_LEXER_AST_HPP
#include "Node.hpp"
#include <string>

namespace Lexer::AST {

EXPR(Number, CTOR(value(std::move(value))), std::string value)

EXPR(String, CTOR(value(std::move(value))), std::string value)

EXPR(Boolean, CTOR(value(std::move(value))), std::string value)

EXPR(RegularExpression, CTOR(value(std::move(value))), std::string value)

EXPR(Array, CTOR(elements(std::move(elements))), std::vector<Expr::Ptr> elements)

using Property = std::pair<std::string, Expr::Ptr>;
EXPR(Object, CTOR(properties(std::move(properties))), std::vector<Property> properties)

EXPR_DEFAULT(Null)
EXPR_DEFAULT(Undefined)
EXPR_DEFAULT(This)

EXPR(Identifier, CTOR(name(std::move(name))), std::string name)

EXPR(Grouped, CTOR(expression(std::move(expression))), Expr::Ptr expression)

EXPR(BinaryExpr, CTOR(left(std::move(left)), op(std::move(op)), right(std::move(right))), Expr::Ptr left, std::string op, Expr::Ptr right)

EXPR(AssignementExpr, CTOR(left(std::move(left)), op(std::move(op)), right(std::move(right))), Expr::Ptr left, std::string op, Expr::Ptr right)

EXPR(UnaryExpr, CTOR(op(std::move(op)), expr(std::move(expr))), std::string op, Expr::Ptr expr)

EXPR(PostfixExpr, CTOR(op(std::move(op)), expr(std::move(expr))), std::string op, Expr::Ptr expr)

EXPR(ConditionalExpr, CTOR(condition(std::move(condition)), trueExpr(std::move(trueExpr)), falseExpr(std::move(falseExpr))), Expr::Ptr condition, Expr::Ptr trueExpr, Expr::Ptr falseExpr)

EXPR(NewExpr, CTOR(callee(std::move(callee)), arguments(std::move(arguments))), Expr::Ptr callee, std::vector<Expr::Ptr> arguments)

EXPR(MemberExpr, CTOR(object(std::move(object)), property(std::move(property)), computed(computed)), Expr::Ptr object, Expr::Ptr property, bool computed)

EXPR(CallExpr, CTOR(callee(std::move(callee)), arguments(std::move(arguments))), Expr::Ptr callee, std::vector<Expr::Ptr> arguments)

EXPR(FunctionExpr, CTOR(name(std::move(name)), params(std::move(params)), body(std::move(body))), std::string name, std::vector<std::string> params, Stmt::Ptr body)

STMT(BlockStmt, CTOR(body(std::move(body))), std::vector<Stmt::Ptr> body)

STMT(VarDecl, CTOR(name(std::move(name)), init(std::move(init))), std::string name, Expr::Ptr init)

STMT(ExpressionStmt, CTOR(expression(std::move(expression))), Expr::Ptr expression)

STMT(IfStmt, CTOR(test(std::move(test)), consequent(std::move(consequent)), alternate(std::move(alternate))), Expr::Ptr test, Stmt::Ptr consequent, Stmt::Ptr alternate)

STMT(WhileStmt, CTOR(test(std::move(test)), body(std::move(body))), Expr::Ptr test, Stmt::Ptr body)

STMT(DoWhileStmt, CTOR(test(std::move(test)), body(std::move(body))), Expr::Ptr test, Stmt::Ptr body)

STMT(ForStmt, CTOR(init(std::move(init)), test(std::move(test)), update(std::move(update)), body(std::move(body))), Stmt::Ptr init, Expr::Ptr test, Expr::Ptr update, Stmt::Ptr body)

STMT(ForInStmt, CTOR(left(std::move(left)), right(std::move(right)), body(std::move(body))), Stmt::Ptr left, Expr::Ptr right, Stmt::Ptr body)

STMT(ContinueStmt, CTOR(label(std::move(label))), std::string label)

STMT(BreakStmt, CTOR(label(std::move(label))), std::string label)

STMT(ReturnStmt, CTOR(argument(std::move(argument))), Expr::Ptr argument)

STMT(WithStmt, CTOR(object(std::move(object)), body(std::move(body))), Expr::Ptr object, Stmt::Ptr body)

STMT(SwitchStmt, CTOR(discriminant(std::move(discriminant)), cases(std::move(cases))), Expr::Ptr discriminant, std::vector<Stmt::Ptr> cases)

STMT(SwitchCaseStmt, CTOR(test(std::move(test)), consequent(std::move(consequent))), Expr::Ptr test, std::vector<Stmt::Ptr> consequent)

STMT(LabeledStmt, CTOR(label(std::move(label)), body(std::move(body))), std::string label, Stmt::Ptr body)

STMT(ThrowStmt, CTOR(argument(std::move(argument))), Expr::Ptr argument)

STMT(TryStmt, CTOR(block(std::move(block)), param(std::move(param)), handler(std::move(handler)), finalizer(std::move(finalizer))), Stmt::Ptr block, std::string param, Stmt::Ptr handler, Stmt::Ptr finalizer)

STMT_DEFAULT(DebuggerStmt)

STMT_DEFAULT(EmptyStmt)

STMT(FunctionDecl, CTOR(name(std::move(name)), params(std::move(params)), body(std::move(body))), std::string name, std::vector<std::string> params, Stmt::Ptr body)

}


#endif //JS_CMP_LEXER_AST_HPP
