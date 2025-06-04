#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <string>
#include <iostream>

#include "Program.hpp"

namespace Lexer {
    struct arg {
        std::string name;
        Types type;
    };
    class Function {
    public:
        Function() = default;

        static Function parse_func(const std::vector<Token>& tokens, size_t size, size_t& i);
        static std::vector<Function> parse(std::vector<Token> &tokens);

        std::string name;
        std::vector<arg> args;
        std::vector<Token> value;
        Types return_type = JS_ANY;


        friend std::ostream &operator<<(std::ostream &os, const Function &function);
    };
}
#endif //FUNCTION_HPP
