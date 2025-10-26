#ifndef JS_CMP_LEXER_PARSER_HPP
#define JS_CMP_LEXER_PARSER_HPP
#include "Node.hpp"
#include "../Tokenizer/Token.hpp"


// Green for __FUNCTION__ / __CLASS__ / __LINE__ , blue for token info
#define DEBUG_PARSER(token) std::cout << "\033[1;32mIn " << __FUNCTION__ << " at line " << __LINE__ << ":\033[0m " \
                                   << "\033[1;34mToken: '" << token.value << "' Type: " << TokenName[token.type] \
                                   << " Line: " << token.line << " Col: " << token.column << "\033[0m\n";

namespace Lexer::AST {
class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message)
        : std::runtime_error("Parser Error: " + message) {
    }
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens)
        : tokens(tokens), cursor(0) {
    }

    ~Parser() = default;

    Stmt::Ptr parseStatement();

    Expr::Ptr parsePrimary();

    Expr::Ptr parseNewOrMember();

    Expr::Ptr parseLeftHandSide();
    Expr::Ptr parsePostfix();
    Expr::Ptr parseUnary();

    Expr::Ptr parseMultiplicative();
    Expr::Ptr parseAdditive();
    Expr::Ptr parseShift();
    Expr::Ptr parseRelational();
    Expr::Ptr parseEquality();
    Expr::Ptr parseBitwiseAnd();
    Expr::Ptr parseBitwiseXor();
    Expr::Ptr parseBitwiseOr();
    Expr::Ptr parseLogicalAnd();
    Expr::Ptr parseLogicalOr();
    Expr::Ptr parseConditional();
    Expr::Ptr parseAssignment();
    Expr::Ptr parseComma();

    Expr::Ptr parseExpression();

    // Token Utils
    bool match(TokenType type, const std::string& lexme = "");
    Token consume(std::function<bool(const Token&)> predicate, const std::string& ParserName, const std::string& errorMessage);
    Token consume(TokenType type, const std::string& ParserName, const std::string& errorMessage);
    const Token& peek() const;
    const Token& advance();
    const Token& reverse();
    const Token& previous() const;
    bool check(TokenType type, const std::string& lexme = "");
    bool matchSemicolon(bool needAdvance = false);
    void expectSemicolon(const std::string& ParserName, const std::string& errorMessage);
    bool isAtEnd() const;
    void skipEols();

private:
    const std::vector<Token>& tokens;
    size_t cursor;
};
}


#endif //JS_CMP_LEXER_PARSER_HPP