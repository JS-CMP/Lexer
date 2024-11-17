#ifndef BODY_HPP
#define BODY_HPP

#include "Token.hpp"
#include <vector>
#include <iostream>

namespace Lexer {

    class Body {
    public:
        Body() = default;

        static bool encapsulate(const Body &body, size_t size, Types &type, std::ostream &os, size_t &i);
        static bool transpileObject(const Body &body, size_t size, Types &type, std::ostream &os, size_t &i);
        static size_t eraseEol(const Body &body, size_t size, size_t &i);


        friend std::ostream &operator<<(std::ostream &os, const Body &body);

        std::vector<Token> value;
        bool no_return = false;

    };
}

#endif //BODY_HPP
