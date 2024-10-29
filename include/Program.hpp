#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <vector>
#include "./Token.hpp"
#include <iostream>
#include <array>

namespace Lexer {
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
}

#include "Function.hpp"
#include "Include.hpp"

namespace Lexer {

    class Program {
    public:
        explicit Program(std::vector<Token> &tokens);

        void parse();
        void parse_func(size_t &i, Function &func);

        static void function_declaration(
            std::ostream &os, const Program &program
        );

        static void include(
            std::ostream &os, const Program &program
        );

        std::vector<Token> tokens;

        std::vector<Include> includes;
        std::vector<Function> functions;

        friend std::ostream &operator<<(
            std::ostream &os, const Program &program
        );
    };
}

#endif //PROGRAM_HPP
