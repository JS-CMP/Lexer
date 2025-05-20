#ifndef BODY_HPP
#define BODY_HPP

#include "Token.hpp"
#include <vector>
#include <iostream>

namespace Lexer {
    class Body {
    public:
        Body() = default;
        explicit Body(const std::vector<Token> &tokens);

        void parse();

        static bool encapsulate(const Body &body, size_t size, std::ostringstream &os, size_t &i);
        static bool transpileBlocks(const Body& body, size_t size, std::ostringstream& os, size_t& i, const std::vector<std::tuple<TokenType, TokenType, bool>> &stack);
        static bool transpileObject(const Body &body, size_t size, std::ostringstream &os, size_t &i);
        static bool transpileArray(const Body &body, size_t size, std::ostringstream &os, size_t &i);
        static size_t eraseEol(const Body &body, size_t size, size_t &i);

        friend std::ostream &operator<<(std::ostream &os, const Body &body);

        std::vector<Token> value;

        // before parse
        std::vector<Function> functions;
    };
}

#endif //BODY_HPP
