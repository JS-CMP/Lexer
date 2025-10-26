#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include "Token.hpp"

namespace Lexer {
    class Lexer {
    public:
        explicit Lexer(std::string &content);
        explicit Lexer(const char *content);

        std::vector<Token> tokenize();
        Token nextToken();

        void leftTrim();

        std::string content;
        size_t cursor;
        size_t line;
        size_t start_of_line;

    };
}

#endif //LEXER_HPP
