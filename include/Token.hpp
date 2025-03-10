#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <array>

namespace Lexer {

    enum TokenType {
        TK_EOS = 0,          // EOS
        TK_ILLEGAL,         // ILLEGAL

        TK_SIMPLE_OP_FIRST, // SIMPLE OP FIRST
        TK_PERIOD,          // .
        TK_COLON,           // :
        TK_SEMICOLON,       // ;
        TK_COMMA,           // ,

        TK_LPAREN,          // (
        TK_RPAREN,          // )
        TK_LBRACK,          // [
        TK_RBRACK,          // ]
        TK_LBRACE,          // {
        TK_RBRACE,          // }

        TK_CONDITIONAL,     // ?
        TK_ADD,             // +
        TK_SUB,             // -
        TK_MUL,             // *
        TK_DIV,             // /
        TK_MOD,             // %
        TK_LT,              // <
        TK_GT,              // >
        TK_BIT_AND,         // &
        TK_BIT_OR,          // |
        TK_BIT_XOR,         // ^
        TK_BIT_NOT,         // ~
        TK_ASSIGN,          // =
        TK_NOT,             // !
        TK_DOUBLE_QUOTE,    // "
        TK_SINGLE_QUOTE,    // '
        TK_BACKTICK,        // `
        TK_SIMPLE_OP_LAST,  // SIMPLE OP LAST
        TK_DOUBLE_OP_FIRST, // DOUBLE OP FIRST
        TK_EQ,              // ==
        TK_NE,              // !=
        TK_INC,             // ++
        TK_DEC,             // --
        TK_LTE,             // <=
        TK_GTE,             // >=
        TK_SAR,             // >>
        TK_SHL,             // <<
        TK_LOGICAL_AND,     // &&
        TK_LOGICAL_OR,      // ||

        TK_ASSIGN_ADD,      // +=
        TK_ASSIGN_SUB,      // -=
        TK_ASSIGN_MUL,      // *=
        TK_ASSIGN_MOD,      // %=
        TK_ASSIGN_DIV,      // /=
        TK_ASSIGN_BIT_AND,  // &=
        TK_ASSIGN_BIT_OR,   // |=
        TK_ASSIGN_BIT_XOR,  // ^=
        TK_EXP,             // **
        TK_DOUBLE_OP_LAST,  // DOUBLE OP LAST
        TK_TRIPLE_OP_FIRST, // TRIPLE OP FIRST
        TK_EQ_STRICT,       // ===
        TK_NE_STRICT,       // !==
        TK_SPREAD,          // ...
        TK_SHR,             // >>>
        TK_TRIPLE_OP_LAST,  // TRIPLE OP LAST
        TK_QUAD_OP_FIRST,   // QUAD OP FIRST
        TK_ASSIGN_SAR,      // >>=
        TK_ASSIGN_SHR,      // >>>=
        TK_ASSIGN_SHL,      // <<=
        TK_QUAD_OP_LAST,    // QUAD OP LAST

        TK_DELETE,          // delete
        TK_TYPEOF,          // typeof
        TK_DEBUGGER,        // debugger
        TK_INSTANCEOF,      // instanceof
        TK_VOID,            // void
        TK_BREAK,           // break
        TK_CONTINUE,        // continue
        TK_CASE,            // case
        TK_IN,              // in
        TK_NEW,             // new
        TK_THIS,            // this
        TK_THROW,           // throw
        TK_WITH,            // with
        TK_CLASS,           // class
        TK_RETURN,          // return
        TK_FIRST_SCOPED,    // FIRST SCOPED
        TK_CATCH,           // catch
        TK_DO,              // do
        TK_ELSE,            // else
        TK_FINALLY,         // finaly
        TK_FOR,             // for
        TK_FUNCTION,        // function
        TK_IF,              // if
        TK_SWITCH,          // switch
        TK_TRY,             // try
        TK_WHILE,           // while
        TK_LAST_SCOPED,     // LAST SCOPED


        TK_STRICT_FIRST,    // STRICT FIRST
        TK_VAR,             // var
        TK_CONST,           // const
        TK_LET,             // let
        TK_STRICT_LAST,     // STRICT LAST

        TK_UNDEFINED_LITERAL, // UNDEFINED LITERAL
        TK_NULL_LITERAL,    // NULL   LITERAL
        TK_FALSE_LITERAL,   // FALSE  LITERAL
        TK_TRUE_LITERAL,    // TRUE   LITERAL
        TK_NUMBER,          // NUMBER LITERAL
        TK_STRING,          // STRING LITERAL
        TK_IDENTIFIER,      // IDENTIFIER
        TK_EOL,             // END OF LINE


        TK_SINGLE_LINE_COMMENT,  // SINGLE LINE COMMENT
        TK_MULTI_LINE_COMMENT,  // MULTI LINE COMMENT

        TK_NOT_FOUND,       // NOT_FOUND

        TK_NUM_TOKENS       // number of tokens
    };

    static const std::array<const char *, TokenType::TK_NUM_TOKENS> TokenValue = {
            nullptr,            // EOS
            nullptr,            // ILLEGAL
            nullptr,            // SIMPLE OP FIRST
            ".",
            ":",
            ";",
            ",",
            "(",
            ")",
            "[",
            "]",
            "{",
            "}",
            "?",
            "+",
            "-",
            "*",
            "/",
            "%",
            "<",
            ">",
            "&",
            "|",
            "^",
            "~",
            "=",
            "!",
            "\"",
            "'",
            "`",
            nullptr,            // SIMPLE OP LAST
            nullptr,            // DOUBLE OP FIRST
            "==",
            "!=",
            "++",
            "--",
            "<=",
            ">=",
            ">>",
            "<<",
            "&&",
            "||",
            "+=",
            "-=",
            "*=",
            "%=",
            "/=",
            "&=",
            "|=",
            "^=",
            "**",
            nullptr,            // DOUBLE OP LAST
            nullptr,            // TRIPLE OP FIRST
            "===",
            "!==",
            "...",
            ">>>",
            nullptr,           // TRIPLE OP LAST
            nullptr,           // QUAD OP FIRST
            ">>=",
            ">>>=",
            "<<=",
            nullptr,           // QUAD OP LAST

            "delete",
            "typeof",
            "instanceof",
            "debugger",
            "void",
            "break",
            "continue",
            "case",
            "in",
            "new",
            "this",
            "throw",
            "with",
            "class",
            "return",
            nullptr, // First Scoped
            "catch",
            "do",
            "else",
            "finally",
            "for",
            "function",
            "if",
            "switch",
            "try",
            "while",
            nullptr, // Last Scoped
            nullptr,// STRICT FIRST
            "var",
            "const",
            "let",
            nullptr,
            "undefined",
            "null",
            "false",
            "true",
            nullptr,            // NUMBER LITERAL
            nullptr,            // STRING LITERAL
            nullptr,            // IDENTIFIER
            "\n",               // END OF LINE
            nullptr             // NOT FOUND
    };

    static const std::array<const char *, TokenType::TK_NUM_TOKENS> TokenName = {
            "TK_EOS",
            "TK_ILLEGAL",
            "TK_SIMPLE_OP_FIRST",
            "TK_PERIOD",
            "TK_COLON",
            "TK_SEMICOLON",
            "TK_COMMA",
            "TK_LPAREN",
            "TK_RPAREN",
            "TK_LBRACK",
            "TK_RBRACK",
            "TK_LBRACE",
            "TK_RBRACE",
            "TK_CONDITIONAL",
            "TK_ADD",
            "TK_SUB",
            "TK_MUL",
            "TK_DIV",
            "TK_MOD",
            "TK_LT",
            "TK_GT",
            "TK_BIT_AND",
            "TK_BIT_OR",
            "TK_BIT_XOR",
            "TK_ASSIGN",
            "TK_NOT",
            "TK_DOUBLE_QUOTE",
            "TK_SINGLE_QUOTE",
            "TK_BACKTICK",
            "TK_SIMPLE_OP_LAST",
            "TK_DOUBLE_OP_FIRST",
            "TK_EQ",
            "TK_NE",
            "TK_INC",
            "TK_DEC",
            "TK_LTE",
            "TK_GTE",
            "TK_SAR",
            "TK_SHL",
            "TK_LOGICAL_AND",
            "TK_LOGICAL_OR",
            "TK_ASSIGN_ADD",
            "TK_ASSIGN_SUB",
            "TK_ASSIGN_MUL",
            "TK_ASSIGN_MOD",
            "TK_ASSIGN_DIV",
            "TK_ASSIGN_BIT_AND",
            "TK_ASSIGN_BIT_OR",
            "TK_ASSIGN_BIT_XOR",
            "TK_EXP",
            "TK_DOUBLE_OP_LAST",
            "TK_TRIPLE_OP_FIRST",
            "TK_EQ_STRICT",
            "TK_NE_STRICT",
            "TK_SPREAD",
            "TK_SHR",
            "TK_TRIPLE_OP_LAST",
            "TK_QUAD_OP_FIRST",
            "TK_ASSIGN_SAR",
            "TK_ASSIGN_SHR",
            "TK_ASSIGN_SHL",
            "TK_QUAD_OP_LAST",
            "TK_DELETE",
            "TK_TYPEOF",
            "TK_INSTANCEOF",
            "TK_VOID",
            "TK_BREAK",
            "TK_CASE",
            "TK_CATCH",
            "TK_CONTINUE",
            "TK_DEBUGGER",
            "TK_DO",
            "TK_ELSE",
            "TK_FINALLY",
            "TK_FOR",
            "TK_FUNCTION",
            "TK_IF",
            "TK_IN",
            "TK_NEW",
            "TK_RETURN",
            "TK_SWITCH",
            "TK_THIS",
            "TK_THROW",
            "TK_TRY",
            "TK_WHILE",
            "TK_WITH",
            "TK_CLASS",
            "TK_STRICT_FIRST",
            "TK_VAR",
            "TK_CONST",
            "TK_LET",
            "TK_STRICT_LAST",
            "TK_UNDEFINED_LITERAL",
            "TK_NULL_LITERAL",
            "TK_FALSE_LITERAL",
            "TK_TRUE_LITERAL",
            "TK_NUMBER",
            "TK_STRING",
            "TK_IDENTIFIER",
            "TK_EOL",
            "TK_SINGLE_LINE_COMMENT",
            "TK_MULTI_LINE_COMMENT",
            "TK_NOT_FOUND"
    };

    enum Types {
        JS_ANY_REF = 0,
        JS_PRIMITIVE_REF,
        JS_NUMBER_REF,
        JS_STRING_REF,
        JS_BOOLEAN_REF,
        JS_NULL_REF,
        JS_UNDEFINED_REF,
        JS_OBJECT_REF,
        JS_ANY,
        JS_PRIMITIVE,
        JS_NUMBER,
        JS_STRING,
        JS_BOOLEAN,
        JS_NULL,
        JS_UNDEFINED,
        JS_OBJECT,
        INT,
        JS_NUM_TYPES
    };

    static const std::array<const char *, JS_NUM_TYPES> TypeNames = {
            "JS::Any &",
            "JS::Primitive &",
            "JS::Number &",
            "JS::String &",
            "JS::Boolean &",
            "JS::Null &",
            "JS::Undefined &",
            "JS::Object &",
            "JS::Any",
            "JS::Primitive",
            "JS::Number",
            "JS::String",
            "JS::Boolean",
            "JS::Null",
            "JS::Undefined",
            "JS::Object",
            "int"
    };

    class Function;


    class Token {
    public:
        Token();

        explicit Token(TokenType type);

        Token(std::string value, TokenType type, unsigned long line, unsigned long column);

        static bool isSkippable(char ch);

        static bool isSymbolStart(char ch);

        static bool isSymbol(char ch);

        static TokenType isKeyword(const std::string &str);

        static bool isAlpha(const std::string &str);

        std::string value;
        TokenType type;
        unsigned long line;
        unsigned long column;
    };
}
#endif //TOKEN_HPP
