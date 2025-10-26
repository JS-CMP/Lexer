#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <array>

#define TOKENS(X) \
    X(TK_EOS, nullptr)          /* END OF STREAM */ \
    X(TK_ILLEGAL, nullptr)      /* ILLEGAL TOKEN */ \
    X(TK_SIMPLE_OP_FIRST, nullptr) /* SIMPLE OP FIRST */ \
    X(TK_PERIOD, ".")      /* . */ \
    X(TK_COLON, ":")      /* : */ \
    X(TK_SEMICOLON, ";")  /* ; */ \
    X(TK_COMMA, ",")      /* , */ \
    X(TK_LPAREN, "(")       /* ( */ \
    X(TK_RPAREN, ")")       /* ) */ \
    X(TK_LBRACK, "[")       /* [ */ \
    X(TK_RBRACK, "]")       /* ] */ \
    X(TK_LBRACE, "{")       /* { */ \
    X(TK_RBRACE, "}")       /* } */ \
    X(TK_CONDITIONAL, "?") /* ? */ \
    X(TK_ADD, "+")          /* + */ \
    X(TK_SUB, "-")          /* - */ \
    X(TK_MUL, "*")          /* * */ \
    X(TK_DIV, "/")          /* / */ \
    X(TK_MOD, "%")          /* % */ \
    X(TK_LT, "<")           /* < */ \
    X(TK_GT, ">")           /* > */ \
    X(TK_BIT_AND, "&")   /* & */ \
    X(TK_BIT_OR, "|")    /* | */ \
    X(TK_BIT_XOR, "^")   /* ^ */ \
    X(TK_BIT_NOT, "~")   /* ~ */ \
    X(TK_ASSIGN, "=")      /* = */ \
    X(TK_NOT, "!")         /* ! */ \
    X(TK_DOUBLE_QUOTE, "\"") /* " */ \
    X(TK_SINGLE_QUOTE, "'") /* ' */ \
    X(TK_BACKTICK, "`")   /* ` */ \
    X(TK_SIMPLE_OP_LAST, nullptr) /* SIMPLE OP LAST */ \
    X(TK_DOUBLE_OP_FIRST, nullptr) /* DOUBLE OP FIRST */ \
    X(TK_EQ, "==")          /* == */ \
    X(TK_NE, "!=")          /* != */ \
    X(TK_INC, "++")          /* ++ */ \
    X(TK_DEC, "--")          /* -- */ \
    X(TK_LTE, "<=")          /* <= */ \
    X(TK_GTE, ">=")          /* >= */ \
    X(TK_SAR, ">>")         /* >> */ \
    X(TK_SHL, "<<")         /* << */ \
    X(TK_LOGICAL_AND, "&&")  /* && */ \
    X(TK_LOGICAL_OR, "||")   /* || */ \
    X(TK_ASSIGN_ADD, "+=")   /* += */ \
    X(TK_ASSIGN_SUB, "-=")   /* -= */ \
    X(TK_ASSIGN_MUL, "*=")   /* *= */ \
    X(TK_ASSIGN_MOD, "%=")   /* %= */ \
    X(TK_ASSIGN_DIV, "/=")   /* /= */ \
    X(TK_ASSIGN_BIT_AND, "&=") /* &= */ \
    X(TK_ASSIGN_BIT_OR, "|=")  /* |= */ \
    X(TK_ASSIGN_BIT_XOR, "^=") /* ^= */ \
    X(TK_EXP, "**")         /* ** */ \
    X(TK_DOUBLE_OP_LAST, nullptr) /* DOUBLE OP LAST */ \
    X(TK_TRIPLE_OP_FIRST, nullptr) /* TRIPLE OP FIRST */ \
    X(TK_EQ_STRICT, "===")  /* === */ \
    X(TK_NE_STRICT, "!==")  /* !== */ \
    X(TK_SHR, ">>>")    /* >>> */ \
    X(TK_TRIPLE_OP_LAST, nullptr) /* TRIPLE OP LAST */ \
    X(TK_QUAD_OP_FIRST, nullptr) /* QUAD OP FIRST */ \
    X(TK_ASSIGN_SAR, ">>=")   /* >>= */ \
    X(TK_ASSIGN_SHR, ">>>=") /* >>>= */ \
    X(TK_ASSIGN_SHL, "<<=") /* <<<= */ \
    X(TK_QUAD_OP_LAST, nullptr) /* QUAD OP LAST */ \
    X(TK_DELETE, "delete")   /* delete */ \
    X(TK_TYPEOF, "typeof")   /* typeof */ \
    X(TK_DEBUGGER, "debugger") /* debugger */ \
    X(TK_INSTANCEOF, "instanceof") /* instanceof */ \
    X(TK_VOID, "void")     /* void */ \
    X(TK_BREAK, "break")   /* break */ \
    X(TK_CONTINUE, "continue") /* continue */ \
    X(TK_CASE, "case")     /* case */ \
    X(TK_DEFAULT, "default") /* default */ \
    X(TK_IN, "in")       /* in */ \
    X(TK_NEW, "new")     /* new */ \
    X(TK_THIS, "this")   /* this */ \
    X(TK_THROW, "throw") /* throw */ \
    X(TK_WITH, "with")   /* with */ \
    X(TK_CLASS, "class") /* class */ \
    X(TK_RETURN, "return") /* return */ \
    X(TK_FIRST_SCOPED, nullptr) /* FIRST SCOPED */ \
    X(TK_CATCH, "catch") /* catch */ \
    X(TK_DO, "do")     /* do */ \
    X(TK_ELSE, "else") /* else */ \
    X(TK_FINALLY, "finally") /* finally */ \
    X(TK_FOR, "for")   /* for */ \
    X(TK_FUNCTION, "function") /* function */ \
    X(TK_IF, "if")     /* if */ \
    X(TK_SWITCH, "switch") /* switch */ \
    X(TK_TRY, "try")   /* try */ \
    X(TK_WHILE, "while") /* while */ \
    X(TK_LAST_SCOPED, nullptr) /* LAST SCOPED */ \
    X(TK_STRICT_FIRST, nullptr) /* STRICT FIRST */ \
    X(TK_VAR, "var")   /* var */ \
    X(TK_STRICT_LAST, nullptr) /* STRICT LAST */ \
    X(TK_UNDEFINED_LITERAL, "undefined") /* UNDEFINED LITERAL */ \
    X(TK_NULL_LITERAL, "null") /* NULL LITERAL */ \
    X(TK_FALSE_LITERAL, "false") /* FALSE  LITERAL */ \
    X(TK_TRUE_LITERAL, "true")   /* TRUE LITERAL */ \
    X(TK_NUMBER, nullptr)    /* NUMBER LITERAL */ \
    X(TK_STRING, nullptr)    /* STRING LITERAL */ \
    X(TK_REGEXP, nullptr)    /* REGEXP LITERAL */ \
    X(TK_IDENTIFIER, nullptr) /* IDENTIFIER */ \
    X(TK_EOL, "\\n")     /* END OF LINE */ \
    X(TK_SINGLE_LINE_COMMENT, "//") /* SINGLE LINE COMMENT */ \
    X(TK_MULTI_LINE_COMMENT, "/* */") /* MULTI LINE COMMENT */ \
    X(TK_NOT_FOUND, nullptr) /* NOT FOUND */ \
    X(TK_NUM_TOKENS, nullptr) /* NUMBER OF TOKENS */

#define GENERATE_ENUM(ENUM, ...) ENUM,
#define GENERATE_STRING(STRING, ...) #STRING,
#define GENERATE_VALUE(VALUE, STRING) STRING,

namespace Lexer {
enum TokenType {
    TOKENS(GENERATE_ENUM)
};

static constexpr std::array<const char*, TK_NUM_TOKENS + 1> TokenString = {
    TOKENS(GENERATE_STRING)
};

static constexpr std::array<const char*, TK_NUM_TOKENS + 1> TokenValue = {
    TOKENS(GENERATE_VALUE)
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

static const std::array<const char*, JS_NUM_TYPES> TypeNames = {
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

    static TokenType isKeyword(const std::string& str);

    static bool isAlpha(const std::string& str);

    std::string value;
    TokenType type;
    unsigned long line;
    unsigned long column;
};
}
#endif //TOKEN_HPP