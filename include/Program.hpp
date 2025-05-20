#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "./Token.hpp"

#include <vector>
#include <iostream>
#include <array>

#include "Body.hpp"
#include "Function.hpp"
#include "Include.hpp"

namespace Lexer {

    class Program {
    public:
        explicit Program(std::vector<Token> &tokens);

        void parse();

        static void include(
            std::ostream &os, const Program &program
        );

        std::vector<Token> tokens;

        std::vector<Include> includes;
        Body main;

        friend std::ostream &operator<<(
            std::ostream &os, const Program &program
        );
    };
}

#endif //PROGRAM_HPP
