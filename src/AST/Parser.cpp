#include "../../include/AST/Parser.hpp"
#include "../../include/AST/AST.hpp"

#include <functional>
#include <iostream>

namespace Lexer::AST {


Stmt::Ptr Parser::parseStatement() {
    skipEols();
    if (this->isAtEnd()) {
        return EmptyStmt::parse(*this);
    }
    if (match(TK_IDENTIFIER)) {
        if (check(TK_COLON)) {
            reverse();
            return LabeledStmt::parse(*this);
        }
        reverse();
    }

    switch (peek().type) {
        case TK_VAR:
            return VarDecl::parse(*this);
        case TK_IF:
            return IfStmt::parse(*this);
        case TK_FOR:
            return ForStmt::parse(*this);
        case TK_WHILE:
            return WhileStmt::parse(*this);
        case TK_DO:
            return DoWhileStmt::parse(*this);
        case TK_FUNCTION:
            return FunctionDecl::parse(*this);
        case TK_RETURN:
            return ReturnStmt::parse(*this);
        case TK_BREAK:
            return BreakStmt::parse(*this);
        case TK_CONTINUE:
            return ContinueStmt::parse(*this);
        case TK_SWITCH:
            return SwitchStmt::parse(*this);
        case TK_THROW:
            return ThrowStmt::parse(*this);
        case TK_TRY:
            return TryStmt::parse(*this);
        case TK_WITH:
            return WithStmt::parse(*this);
        case TK_DEBUGGER:
            return DebuggerStmt::parse(*this);
        case TK_LBRACE:
            return BlockStmt::parse(*this);
        default:
            return ExpressionStmt::parse(*this);
    }
}

Expr::Ptr Parser::parsePrimary() {
    switch (peek().type) {
        case TK_THIS:
            return This::parse(*this);
        case TK_IDENTIFIER:
            return Identifier::parse(*this);
        case TK_NUMBER:
            return Number::parse(*this);
        case TK_STRING:
            return String::parse(*this);
        case TK_TRUE_LITERAL:
        case TK_FALSE_LITERAL:
            return Boolean::parse(*this);
        case TK_NULL_LITERAL:
            return Null::parse(*this);
        case TK_UNDEFINED_LITERAL:
            return Undefined::parse(*this);
        case TK_REGEXP:
            return RegularExpression::parse(*this);
        case TK_FUNCTION:
            return FunctionExpr::parse(*this);
        case TK_LBRACK:
            return Array::parse(*this);
        case TK_LBRACE:
            return Object::parse(*this);
        case TK_LPAREN:
            return Grouped::parse(*this);
        default:
            throw ParseError("Unexpected token in primary expression: " + peek().value + " of type " + TokenName[peek().type] + " at line " + std::to_string(peek().line) + " col " + std::to_string(peek().column) + ".");
    }
}

Expr::Ptr Parser::parseNewOrMember() {
    Expr::Ptr expr;
    if (match(TK_NEW)) {
        expr = NewExpr::parse(*this);
    } else {
        expr = parsePrimary();
    }
    while (true) {
        if (match(TK_PERIOD) || match(TK_LBRACK)) {
            expr = MemberExpr::parse(*this, std::move(expr));
        } else {
            break;
        }
    }


    return expr;
}

Expr::Ptr Parser::parseLeftHandSide() {
    Expr::Ptr expr = parseNewOrMember();
    while (true) {
        if (match(TK_PERIOD) || match(TK_LBRACK)) {
            expr = MemberExpr::parse(*this, std::move(expr));
        } else if (match(TK_LPAREN)) {
            expr = CallExpr::parse(*this, std::move(expr));
        } else {
            break;
        }
    }
    return expr;
}

Expr::Ptr Parser::parsePostfix() {
    Expr::Ptr expr = parseLeftHandSide();

    if (matchSemicolon()) {
        return expr;
    }

    if (match(TK_INC) || match(TK_DEC)) {
        expr = PostfixExpr::parse(*this, std::move(expr));
    }
    return expr;
}

Expr::Ptr Parser::parseUnary() {
    switch (peek().type) {
        case TK_INC:
        case TK_DEC:
        case TK_DELETE:
        case TK_VOID:
        case TK_TYPEOF:
        case TK_ADD:
        case TK_SUB:
        case TK_BIT_NOT:
        case TK_NOT: {
            return UnaryExpr::parse(*this);
        }
        default:
            return parsePostfix();
    }
}

Expr::Ptr Parser::parseMultiplicative() {
    Expr::Ptr expr = parseUnary();
    while (match(TK_MUL) || match(TK_DIV) || match(TK_MOD)) {
        Token opTok = previous();
        Expr::Ptr right = parseUnary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseAdditive() {
    Expr::Ptr expr = parseMultiplicative();
    while (match(TK_ADD) || match(TK_SUB)) {
        Token opTok = previous();
        Expr::Ptr right = parseMultiplicative();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseShift() {
    Expr::Ptr expr = parseAdditive();
    while (match(TK_SAR) || match(TK_SHL) || match(TK_SHR)) {
        Token opTok = previous();
        Expr::Ptr right = parseAdditive();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseRelational() {
    Expr::Ptr expr = parseShift();
    while (match(TK_LT) || match(TK_GT) || match(TK_LTE) || match(TK_GTE) || match(TK_IN) || match(TK_INSTANCEOF)) {
        Token opTok = previous();
        Expr::Ptr right = parseShift();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseEquality() {
    Expr::Ptr expr = parseRelational();
    while (match(TK_EQ) || match(TK_NE) || match(TK_EQ_STRICT) || match(TK_NE_STRICT)) {
        Token opTok = previous();
        Expr::Ptr right = parseRelational();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseBitwiseAnd() {
    Expr::Ptr expr = parseEquality();
    while (match(TK_BIT_AND)) {
        Token opTok = previous();
        Expr::Ptr right = parseEquality();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseBitwiseXor() {
    Expr::Ptr expr = parseBitwiseAnd();
    while (match(TK_BIT_XOR)) {
        Token opTok = previous();
        Expr::Ptr right = parseBitwiseAnd();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseBitwiseOr() {
    Expr::Ptr expr = parseBitwiseXor();
    while (match(TK_BIT_OR)) {
        Token opTok = previous();
        Expr::Ptr right = parseBitwiseXor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseLogicalAnd() {
    Expr::Ptr expr = parseBitwiseOr();
    while (match(TK_LOGICAL_AND)) {
        Token opTok = previous();
        Expr::Ptr right = parseBitwiseOr();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseLogicalOr() {
    Expr::Ptr expr = parseLogicalAnd();
    while (match(TK_LOGICAL_OR)) {
        Token opTok = previous();
        Expr::Ptr right = parseLogicalAnd();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseConditional() {
    Expr::Ptr expr = parseLogicalOr();
    if (match(TK_CONDITIONAL)) {
        expr = ConditionalExpr::parse(*this, std::move(expr));
    }
    return expr;
}

Expr::Ptr Parser::parseAssignment() {
    Expr::Ptr expr = parseConditional();
    if (match(TK_ASSIGN) || match(TK_ASSIGN_ADD) || match(TK_ASSIGN_SUB) || match(TK_ASSIGN_MUL) ||
        match(TK_ASSIGN_DIV) || match(TK_ASSIGN_MOD) || match(TK_ASSIGN_BIT_AND) || match(TK_ASSIGN_BIT_OR) ||
        match(TK_ASSIGN_BIT_XOR) || match(TK_ASSIGN_SAR) || match(TK_ASSIGN_SHL) || match(TK_ASSIGN_SHR)) {
        Token opTok = previous();
        Expr::Ptr right = parseAssignment();
        expr = std::make_unique<AssignementExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseComma() {
    Expr::Ptr expr = parseAssignment();
    while (match(TK_COMMA)) {
        Token opTok = previous();
        Expr::Ptr right = parseAssignment();
        expr = std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
    }
    return expr;
}

Expr::Ptr Parser::parseExpression() {
    return parseComma();
}

bool Parser::match(TokenType type, const std::string& lexme) {
    if (check(type, lexme)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(std::function<bool(const Token&)> predicate, const std::string& ParserName, const std::string& errorMessage) {
    if (predicate(peek())) {
        return advance();
    }
    throw ParseError("[" + ParserName + "] " + errorMessage + " at line " + std::to_string(peek().line) + " col " + std::to_string(peek().column) + ". Found '" + peek().value + "'");
}

Token Parser::consume(TokenType type, const std::string& ParserName, const std::string& errorMessage) {
    if (check(type)) {
        return advance();
    }
    throw ParseError("[" + ParserName + "] " + errorMessage + " at line " + std::to_string(peek().line) + " col " + std::to_string(peek().column) + ". Found '" + peek().value + "'");
}

const Token& Parser::previous() const {
    return tokens[cursor - 1];
}

const Token& Parser::peek() const {
    if (cursor >= tokens.size() - 1) {
        return tokens.back();
    }
    return tokens[cursor];
}

const Token& Parser::advance() {
    if (!isAtEnd()) {
        cursor++;
    }
    return previous();
}

const Token& Parser::reverse() {
    if (cursor > 0) {
        cursor--;
    }
    return peek();
}

bool Parser::check(TokenType type, const std::string& lexme) {
    if (isAtEnd()) {
        return false;
    }
    skipEols();
    const Token& token = peek();
    return token.type == type && (lexme.empty() || token.value == lexme);
}

bool Parser::matchSemicolon(bool needAdvance) {
    // https://262.ecma-international.org/5.1/#sec-7.9.1

    // 2
    if (isAtEnd()) {
        return true;
    }
    if (check(TK_SEMICOLON)) {
        if (needAdvance)
            advance();
        return true;
    }

    // 1
    if (previous().type == TK_EOL || peek().type == TK_RBRACE) {
        return true;
    }
    return false;
}

void Parser::expectSemicolon(const std::string& ParserName, const std::string& errorMessage) {
    if (!matchSemicolon(true)) {
        throw ParseError("[" + ParserName + "] " + errorMessage + " at line " + std::to_string(peek().line) + " col " + std::to_string(peek().column) + ". Found '" + peek().value + "'.");
    }
}

bool Parser::isAtEnd() const {
    return cursor >= tokens.size() || peek().type == TK_EOS;
}

void Parser::skipEols() {
    while (!isAtEnd() && peek().type == TK_EOL) {
        advance();
    }
}
}