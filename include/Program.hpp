#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "./Token.hpp"

#include <vector>
#include <iostream>
#include <array>

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
