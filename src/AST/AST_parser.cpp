#include "../../include/AST/AST.hpp"
#include "AST/Parser.hpp"
#include "Tokenizer/Token.hpp"

#include <iostream>

namespace Lexer::AST {

Expr::Ptr Number::parse(Parser& parser, Expr::Ptr expr) {
    Token numTok = parser.consume(TK_NUMBER, "Number", "Expected number literal.");
    return std::make_unique<Number>(numTok.value);
}

Expr::Ptr String::parse(Parser& parser, Expr::Ptr expr) {
    Token strTok = parser.consume(TK_STRING, "String", "Expected string literal.");
    return std::make_unique<String>(strTok.value);
}

Expr::Ptr Boolean::parse(Parser& parser, Expr::Ptr expr) {
    Token boolTok = parser.consume([](const Token& tok) {
        return tok.type == TK_TRUE_LITERAL || tok.type == TK_FALSE_LITERAL;
    }, "Boolean", "Expected boolean literal.");
    return std::make_unique<Boolean>(boolTok.value);
}

Expr::Ptr RegularExpression::parse(Parser& parser, Expr::Ptr expr) {
    Token regexTok = parser.consume(TK_REGEXP, "RegularExpression", "Expected regular expression literal.");
    return std::make_unique<RegularExpression>(regexTok.value);
}

Expr::Ptr Object::parse(Parser& parser, Expr::Ptr expr) {
    parser.consume(TK_LBRACE, "Object", "Expected '{' at start of object literal.");
    std::vector<Property> properties;
    if (!parser.check(TK_RBRACE)) {
        do {
            Token keyTok = parser.consume([](const Token& tok) {
                return tok.type == TK_STRING || tok.type == TK_IDENTIFIER;
            }, "Object", "Expected string or identifier as object key.");
            parser.consume(TK_COLON, "Object", "Expected ':' after object key.");
            Expr::Ptr value = parser.parseAssignment();
            properties.emplace_back(keyTok.value, std::move(value));
        } while (parser.match(TK_COMMA) && !parser.check(TK_RBRACE));
    }
    parser.consume(TK_RBRACE, "Object", "Expected '}' at end of object literal.");
    return std::make_unique<Object>(std::move(properties));
}

Expr::Ptr Array::parse(Parser& parser, Expr::Ptr expr) {
    parser.consume(TK_LBRACK, "Array", "Expected '[' at start of array literal.");
    std::vector<Expr::Ptr> elements;
    if (!parser.check(TK_RBRACK)) {
        do {
            if (parser.check(TK_COMMA)) {
                elements.push_back(std::make_unique<Undefined>());
                continue;
            }
            elements.push_back(parser.parseAssignment());
        } while (parser.match(TK_COMMA) && !parser.check(TK_RBRACK));
    }
    parser.consume(TK_RBRACK, "Array", "Expected ']' at end of array literal.");
    return std::make_unique<Array>(std::move(elements));
}

Expr::Ptr Null::parse(Parser& parser) {
    parser.consume(TK_NULL_LITERAL, "Null", "Expected null literal.");
    return std::make_unique<Null>();
}

Expr::Ptr Undefined::parse(Parser& parser) {
    parser.consume(TK_UNDEFINED_LITERAL, "Undefined", "Expected undefined literal.");
    return std::make_unique<Undefined>();
}

Expr::Ptr This::parse(Parser& parser) {
    parser.consume(TK_THIS, "This", "Expected 'this' keyword.");
    return std::make_unique<This>();
}

Expr::Ptr Grouped::parse(Parser& parser, Expr::Ptr expr) {
    parser.consume(TK_LPAREN, "Grouped", "Expected '(' at start of grouped expression.");
    Expr::Ptr group = parser.parseExpression();
    parser.consume(TK_RPAREN, "Grouped", "Expected ')' at end of grouped expression.");
    return std::make_unique<Grouped>(std::move(group));
}

Expr::Ptr Identifier::parse(Parser& parser, Expr::Ptr expr) {
    Token idTok = parser.consume(TK_IDENTIFIER, "Identifier", "Expected identifier.");
    return std::make_unique<Identifier>(idTok.value);
}

Expr::Ptr BinaryExpr::parse(Parser& parser, Expr::Ptr expr) {
    Token opTok = parser.consume([](const Token& tok) {
        return tok.type >= TK_ADD && tok.type < TK_QUAD_OP_LAST &&
               !(tok.type == TK_NOT || tok.type == TK_INC || tok.type == TK_DEC);
    }, "BinaryExpr", "Expected binary operator.");
    if ((opTok.type == TK_ASSIGN ||
         (opTok.type >= TK_ASSIGN_ADD && opTok.type <= TK_ASSIGN_BIT_XOR) ||
         (opTok.type >= TK_ASSIGN_SAR && opTok.type <= TK_ASSIGN_SHL)) &&
        dynamic_cast<Identifier*>(expr.get()) == nullptr) {
        throw ParseError("[BinaryExpr] Invalid left-hand side in assignment at line " + std::to_string(opTok.line) + " col " + std::to_string(opTok.column) + ".");
    }
    Expr::Ptr right = parser.parseExpression();
    return std::make_unique<BinaryExpr>(std::move(expr), opTok.value, std::move(right));
}

Expr::Ptr UnaryExpr::parse(Parser& parser, Expr::Ptr expr) {
    Token opTok = parser.consume([](const Token& tok) {
        return tok.type == TK_DEC || tok.type == TK_INC || tok.type == TK_DELETE ||
               tok.type == TK_VOID || tok.type == TK_TYPEOF || tok.type == TK_ADD ||
               tok.type == TK_SUB || tok.type == TK_BIT_NOT || tok.type == TK_NOT;
    }, "UnaryExpr", "Expected unary operator.");
    return std::make_unique<UnaryExpr>(opTok.value, std::move(parser.parseUnary()));
}

Expr::Ptr PostfixExpr::parse(Parser& parser, Expr::Ptr expr) {
    if (parser.previous().type != TK_INC && parser.previous().type != TK_DEC) {
        throw ParseError("[PostfixExpr] Expected '++' or '--' in postfix expression at line " + std::to_string(parser.peek().line) + " col " + std::to_string(parser.peek().column) + ".");
    }
    return std::make_unique<PostfixExpr>(parser.previous().value, std::move(expr));
}

Expr::Ptr ConditionalExpr::parse(Parser& parser, Expr::Ptr expr) {
    if (parser.previous().type != TK_CONDITIONAL) {
        throw ParseError("Expected '?' in conditional expression at line " + std::to_string(parser.peek().line) + " col " + std::to_string(parser.peek().column) + ".");
    }
    Expr::Ptr trueExpr = parser.parseExpression();
    parser.consume(TK_COLON, "ConditionalExpr", "Expected ':' in conditional expression.");
    Expr::Ptr falseExpr = parser.parseExpression();
    return std::make_unique<ConditionalExpr>(std::move(expr), std::move(trueExpr), std::move(falseExpr));
}

Expr::Ptr NewExpr::parse(Parser& parser, Expr::Ptr expr) {
    Expr::Ptr callee = expr ? std::move(expr) : parser.parseNewOrMember();
    std::vector<Expr::Ptr> arguments;
    if (parser.match(TK_LPAREN)) {
        if (!parser.check(TK_RPAREN)) {
            do {
                arguments.push_back(parser.parseExpression());
            } while (parser.match(TK_COMMA));
        }
        parser.consume(TK_RPAREN, "NewExpr", "Expected ')' after arguments.");
    }
    return std::make_unique<NewExpr>(std::move(callee), std::move(arguments));
}

Expr::Ptr MemberExpr::parse(Parser& parser, Expr::Ptr expr) {
    bool computed = false;
    if (parser.previous().type == TK_PERIOD) {
        computed = true;
        Expr::Ptr property = Identifier::parse(parser);
        return std::make_unique<MemberExpr>(std::move(expr), std::move(property), computed);
    }
    if (parser.previous().type == TK_LBRACK) {
        computed = false;
        Expr::Ptr property = parser.parseExpression();
        parser.consume(TK_RBRACK, "MemberExpr", "Expected ']' after property expression.");
        return std::make_unique<MemberExpr>(std::move(expr), std::move(property), computed);
    }
    throw ParseError("Expected '.' or '[' after object expression in MemberExpr at line " + std::to_string(parser.peek().line) + " col " + std::to_string(parser.peek().column) + ".");
}

Expr::Ptr CallExpr::parse(Parser& parser, Expr::Ptr expr) {
    if (parser.previous().type != TK_LPAREN) {
        throw ParseError("Expected '(' after callee expression in CallExpr at line " + std::to_string(parser.peek().line) + " col " + std::to_string(parser.peek().column) + ".");
    }
    std::vector<Expr::Ptr> arguments;
    if (!parser.check(TK_RPAREN)) {
        do {
            arguments.push_back(parser.parseAssignment());
        } while (parser.match(TK_COMMA));
    }
    parser.consume(TK_RPAREN, "CallExpr", "Expected ')' after arguments.");
    return std::make_unique<CallExpr>(std::move(expr), std::move(arguments));
}

Expr::Ptr FunctionExpr::parse(Parser& parser, Expr::Ptr expr) {
    parser.consume(TK_FUNCTION, "FunctionExpr", "Expected 'function' keyword.");
    std::string name;
    if (parser.check(TK_IDENTIFIER)) {
        name = parser.consume(TK_IDENTIFIER, "FunctionExpr", "Expected function name.").value;
    } else {
        name = "Anonymous";
    }
    parser.consume(TK_LPAREN, "FunctionExpr", "Expected '(' after function name.");
    std::vector<std::string> params;
    if (!parser.check(TK_RPAREN)) {
        do {
            Token paramTok = parser.consume(TK_IDENTIFIER, "FunctionExpr", "Expected parameter name.");
            params.push_back(paramTok.value);
        } while (parser.match(TK_COMMA));
    }
    parser.consume(TK_RPAREN, "FunctionExpr", "Expected ')' after parameters.");
    Stmt::Ptr body = BlockStmt::parse(parser);
    return std::make_unique<FunctionExpr>(name, std::move(params), std::move(body));
}

Stmt::Ptr BlockStmt::parse(Parser& parser) {
    parser.consume(TK_LBRACE, "BlockStmt", "Expected '{' at start of block.");
    std::vector<Stmt::Ptr> body;
    while (!parser.check(TK_RBRACE) && !parser.isAtEnd()) {
        body.push_back(parser.parseStatement());
    }
    parser.consume(TK_RBRACE, "BlockStmt", "Expected '}' at end of block.");
    return std::make_unique<BlockStmt>(std::move(body));
}

Stmt::Ptr VarDecl::parse(Parser& parser) {
    parser.consume(TK_VAR, "VarDecl", "Expected 'var' keyword.");
    Token nameTok = parser.consume(TK_IDENTIFIER, "VarDecl", "Expected variable name.");
    Expr::Ptr init;
    if (parser.match(TK_ASSIGN)) {
        init = parser.parseExpression();
    }
    parser.expectSemicolon("VarDecl", "Expected ';' after variable declaration.");
    return std::make_unique<VarDecl>(nameTok.value, std::move(init));
}

Stmt::Ptr ExpressionStmt::parse(Parser& parser) {
    Expr::Ptr expr = parser.parseExpression();
    parser.expectSemicolon("ExpressionStmt", "Expected ';' after expression.");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

Stmt::Ptr IfStmt::parse(Parser& parser) {
    parser.consume(TK_IF, "IfStmt", "Expected 'if' keyword.");
    parser.consume(TK_LPAREN, "IfStmt", "Expected '(' after 'if'.");
    Expr::Ptr test = parser.parseExpression();
    parser.consume(TK_RPAREN, "IfStmt", "Expected ')' after condition.");
    Stmt::Ptr consequent = parser.parseStatement();
    Stmt::Ptr alternate = nullptr;
    if (parser.match(TK_ELSE)) {
        alternate = parser.parseStatement();
    }
    return std::make_unique<IfStmt>(std::move(test), std::move(consequent), std::move(alternate));
}

Stmt::Ptr WhileStmt::parse(Parser& parser) {
    parser.consume(TK_WHILE, "WhileStmt", "Expected 'while' keyword.");
    parser.consume(TK_LPAREN, "WhileStmt", "Expected '(' after 'while'.");
    Expr::Ptr test = parser.parseExpression();
    parser.consume(TK_RPAREN, "WhileStmt", "Expected ')' after condition.");
    Stmt::Ptr body = parser.parseStatement();
    return std::make_unique<WhileStmt>(std::move(test), std::move(body));
}

Stmt::Ptr DoWhileStmt::parse(Parser& parser) {
    parser.consume(TK_DO, "DoWhileStmt", "Expected 'do' keyword.");
    Stmt::Ptr body = parser.parseStatement();
    parser.consume(TK_WHILE, "DoWhileStmt", "Expected 'while' keyword after 'do' body.");
    parser.consume(TK_LPAREN, "DoWhileStmt", "Expected '(' after 'while'.");
    Expr::Ptr test = parser.parseExpression();
    parser.consume(TK_RPAREN, "DoWhileStmt", "Expected ')' after condition.");
    parser.expectSemicolon("DoWhileStmt", "Expected ';' after do-while statement.");
    return std::make_unique<DoWhileStmt>(std::move(test), std::move(body));
}

Stmt::Ptr ForStmt::parse(Parser& parser) {

    parser.consume(TK_FOR, "ForStmt", "Expected 'for' keyword.");
    parser.consume(TK_LPAREN, "ForStmt", "Expected '(' after 'for' .");

    size_t backtrack = 0;
    Token nextTok = parser.peek();
    for (; nextTok.type != TK_SEMICOLON && nextTok.type != TK_IN && nextTok.type != TK_RPAREN && !parser.isAtEnd(); backtrack++) {
        parser.advance();
        if (parser.check(TK_IN)) {
            // It's a for-in loop, backtrack and parse as ForInStmt
            for (size_t i = 0; i < backtrack + 3; i++) {
                parser.reverse();
            }
            return ForInStmt::parse(parser);
        }
        nextTok = parser.peek();
    }
    for (size_t i = 0; i < backtrack; i++) {
        parser.reverse();
    }

    Stmt::Ptr init = nullptr;
    if (!parser.check(TK_SEMICOLON)) {
        init = parser.parseStatement();
    } else {
        parser.advance(); // consume the semicolon
    }
    Expr::Ptr test = nullptr;
    if (!parser.check(TK_SEMICOLON)) {
        test = parser.parseExpression();
    }
    parser.consume(TK_SEMICOLON, "ForStmt", "Expected ';' after for clauses.");
    Expr::Ptr update = nullptr;
    if (!parser.check(TK_RPAREN)) {
        update = parser.parseExpression();
    }
    parser.consume(TK_RPAREN, "ForStmt", "Expected ')' after for clauses.");
    Stmt::Ptr body = parser.parseStatement();
    return std::make_unique<ForStmt>(std::move(init), std::move(test), std::move(update), std::move(body));
}

Stmt::Ptr ForInStmt::parse(Parser& parser) {
    parser.consume(TK_FOR, "ForInStmt", "Expected 'for' keyword.");
    parser.consume(TK_LPAREN, "ForInStmt", "Expected '(' after 'for'.");
    Stmt::Ptr left;
    if (parser.match(TK_VAR)) {
        Token nameTok = parser.consume(TK_IDENTIFIER, "ForInStmt", "Expected variable name in for-in statement.");
        left = std::make_unique<VarDecl>(nameTok.value, nullptr);
    } else {
        Expr::Ptr ptr = parser.parseLeftHandSide();
        left = std::make_unique<ExpressionStmt>(std::move(ptr));
    }
    parser.consume(TK_IN, "ForInStmt", "Expected 'in' in for-in statement.");
    Expr::Ptr right = parser.parseExpression();
    parser.consume(TK_RPAREN, "ForInStmt", "Expected ')' after for-in clauses.");
    Stmt::Ptr body = parser.parseStatement();
    return std::make_unique<ForInStmt>(std::move(left), std::move(right), std::move(body));
}

Stmt::Ptr ContinueStmt::parse(Parser& parser) {
    parser.consume(TK_CONTINUE, "ContinueStmt", "Expected 'continue' keyword.");
    if (parser.peek().type == TK_EOL || parser.match(TK_SEMICOLON)) {
        return std::make_unique<ContinueStmt>(std::string());
    }
    parser.consume(TK_IDENTIFIER, "ContinueStmt", "Expected label after 'continue'.");
    parser.expectSemicolon("ContinueStmt", "Expected ';' after continue statement.");
    return std::make_unique<ContinueStmt>(parser.previous().value);
}

Stmt::Ptr BreakStmt::parse(Parser& parser) {
    parser.consume(TK_BREAK, "BreakStmt", "Expected 'break' keyword.");
    if (parser.peek().type == TK_EOL || parser.match(TK_SEMICOLON)) {
        return std::make_unique<BreakStmt>(std::string());
    }
    parser.consume(TK_IDENTIFIER, "BreakStmt", "Expected label after 'break'.");
    parser.expectSemicolon("BreakStmt", "Expected ';' after break statement.");
    return std::make_unique<BreakStmt>(parser.previous().value);
}

Stmt::Ptr ReturnStmt::parse(Parser& parser) {
    parser.consume(TK_RETURN, "ReturnStmt", "Expected 'return' keyword.");
    if (parser.peek().type == TK_EOL || parser.match(TK_SEMICOLON)) {
        return std::make_unique<ReturnStmt>(nullptr);
    }
    Expr::Ptr argument = parser.parseExpression();
    parser.expectSemicolon("ReturnStmt", "Expected ';' after return statement.");
    return std::make_unique<ReturnStmt>(std::move(argument));
}

Stmt::Ptr ThrowStmt::parse(Parser& parser) {
    parser.consume(TK_THROW, "ThrowStmt", "Expected 'throw' keyword.");
    if (parser.peek().type == TK_EOL || parser.match(TK_SEMICOLON)) {
        return std::make_unique<ThrowStmt>(std::make_unique<Undefined>());
    }
    Expr::Ptr argument = parser.parseExpression();
    parser.expectSemicolon("ThrowStmt", "Expected ';' after throw statement.");
    return std::make_unique<ThrowStmt>(std::move(argument));
}

Stmt::Ptr WithStmt::parse(Parser& parser) {
    throw ParseError("[WithStmt] 'with' statements are not supported due to Strict Mode restrictions.");
}

Stmt::Ptr SwitchStmt::parse(Parser& parser) {
    parser.consume(TK_SWITCH, "SwitchStmt", "Expected 'switch' keyword.");
    parser.consume(TK_LPAREN, "SwitchStmt", "Expected '(' after 'switch'.");
    Expr::Ptr discriminant = parser.parseExpression();
    parser.consume(TK_RPAREN, "SwitchStmt", "Expected ')' after switch expression.");
    parser.consume(TK_LBRACE, "SwitchStmt", "Expected '{' at start of switch body.");
    std::vector<Stmt::Ptr> cases;
    while (!parser.check(TK_RBRACE) && !parser.isAtEnd()) {
        if (parser.match(TK_CASE)) {
            Expr::Ptr test = parser.parseExpression();
            parser.consume(TK_COLON, "SwitchStmt", "Expected ':' after case expression.");
            std::vector<Stmt::Ptr> consequent;
            while (!parser.check(TK_CASE) && !parser.check(TK_DEFAULT) && !parser.check(TK_RBRACE) && !parser.isAtEnd()) {
                consequent.push_back(parser.parseStatement());
            }
            cases.push_back(std::make_unique<SwitchCaseStmt>(std::move(test), std::move(consequent)));
        } else if (parser.match(TK_DEFAULT)) {
            parser.consume(TK_COLON, "SwitchStmt", "Expected ':' after default.");
            std::vector<Stmt::Ptr> consequent;
            while (!parser.check(TK_CASE) && !parser.check(TK_DEFAULT) && !parser.check(TK_RBRACE) && !parser.isAtEnd()) {
                consequent.push_back(parser.parseStatement());
            }
            cases.push_back(std::make_unique<SwitchCaseStmt>(nullptr, std::move(consequent)));
        } else {
            throw ParseError("Expected 'case' or 'default' in switch statement at line " + std::to_string(parser.peek().line) + " col " + std::to_string(parser.peek().column) + ".");
        }
    }
    parser.consume(TK_RBRACE, "SwitchStmt", "Expected '}' at end of switch body.");
    return std::make_unique<SwitchStmt>(std::move(discriminant), std::move(cases));
}

Stmt::Ptr SwitchCaseStmt::parse(Parser& parser) {
    parser.consume(TK_CASE, "SwitchCaseStmt", "Expected 'case' keyword.");
    Expr::Ptr test = parser.parseExpression();
    parser.consume(TK_COLON, "SwitchCaseStmt", "Expected ':' after case expression.");
    std::vector<Stmt::Ptr> consequent;
    while (!parser.check(TK_CASE) && !parser.check(TK_DEFAULT) && !parser.check(TK_RBRACE) && !parser.isAtEnd()) {
        consequent.push_back(parser.parseStatement());
    }
    return std::make_unique<SwitchCaseStmt>(std::move(test), std::move(consequent));
}

Stmt::Ptr LabeledStmt::parse(Parser& parser) {
    Token labelTok = parser.consume(TK_IDENTIFIER, "LabeledStmt", "Expected identifier as label.");
    parser.consume(TK_COLON, "LabeledStmt", "Expected ':' after label.");
    Stmt::Ptr body = parser.parseStatement();
    return std::make_unique<LabeledStmt>(labelTok.value, std::move(body));
}

Stmt::Ptr TryStmt::parse(Parser& parser) {
    parser.consume(TK_TRY, "TryStmt", "Expected 'try' keyword.");
    Stmt::Ptr block = BlockStmt::parse(parser);
    std::string param;
    Stmt::Ptr handler = nullptr;
    if (parser.match(TK_CATCH)) {
        parser.consume(TK_LPAREN, "TryStmt", "Expected '(' after 'catch'.");
        Token paramTok = parser.consume(TK_IDENTIFIER, "TryStmt", "Expected identifier as catch parameter.");
        param = paramTok.value;
        parser.consume(TK_RPAREN, "TryStmt", "Expected ')' after catch parameter.");
        handler = BlockStmt::parse(parser);
    }
    Stmt::Ptr finalizer = nullptr;
    if (parser.match(TK_FINALLY)) {
        finalizer = BlockStmt::parse(parser);
    }
    if (!handler && !finalizer) {
        throw ParseError("Expected 'catch' or 'finally' after 'try' block at line " + std::to_string(parser.peek().line) + " col " + std::to_string(parser.peek().column) + ".");
    }
    return std::make_unique<TryStmt>(std::move(block), param, std::move(handler), std::move(finalizer));
}

Stmt::Ptr DebuggerStmt::parse(Parser& parser) {
    return std::make_unique<DebuggerStmt>();
}

Stmt::Ptr EmptyStmt::parse(Parser& parser) {
    return std::make_unique<EmptyStmt>();
}

Stmt::Ptr FunctionDecl::parse(Parser& parser) {
    parser.consume(TK_FUNCTION, "FunctionDecl", "Expected 'function' keyword.");
    Token nameTok = parser.consume(TK_IDENTIFIER, "FunctionDecl", "Expected function name.");
    parser.consume(TK_LPAREN, "FunctionDecl", "Expected '(' after function name.");
    std::vector<std::string> params;
    if (!parser.check(TK_RPAREN)) {
        do {
            Token paramTok = parser.consume(TK_IDENTIFIER, "FunctionDecl", "Expected parameter name.");
            params.push_back(paramTok.value);
        } while (parser.match(TK_COMMA));
    }
    parser.consume(TK_RPAREN, "FunctionDecl", "Expected ')' after parameters.");
    Stmt::Ptr body = BlockStmt::parse(parser);
    return std::make_unique<FunctionDecl>(nameTok.value, std::move(params), std::move(body));
}

} // namespace Lexer::AST