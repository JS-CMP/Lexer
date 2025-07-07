#include "../include/Function.hpp"
#include "Body.hpp"


namespace Lexer {
    std::ostream &operator<<(
        std::ostream &os, const Function &function
    )
    {
        os << "JS::Any(JS::Function([&](const JS::Any &thisArg, const JS::Any &arguments) -> " << TypeNames[function.return_type] << " {";
        for (size_t i = 0; i < function.args.size(); i++) {
            os << TypeNames[function.args[i].type] << " " << function.args[i].name << " = arguments[\"" << i << "\"];";
        }
        Body body;
        body.value = function.value;
        body.parse();
        os << body;
        // TODO Backtrack to find if the function already has a return statement
        os << "return JS::Any();";
        os << "}))"; // TODO add name and length
        return os;
    }


    Function Function::parse_func(const std::vector<Token>& tokens, size_t size, size_t& i) {
        Function func;
        size_t nb_brace = 1;
        while (tokens[i].type != TK_LPAREN && i < size) {
            i++;
        }
        if (tokens[i - 1].type == TK_IDENTIFIER) {
            func.name = tokens[i - 1].value;
        } else {
            func.name = "anonymous";
        }
        i++;
        func.return_type = JS_ANY;
        while (tokens[i].type != TK_RPAREN) {
            if (tokens[i].type == TK_IDENTIFIER) {
                arg a;
                a.type = JS_ANY;
                a.name = tokens[i].value;
                func.args.push_back(a);
            }
            i++;
        }
        i+= 2;
        while (nb_brace != 0 && i < size) {
            func.value.push_back(tokens[i]);
            i++;
            if (tokens[i].type == TK_RBRACE) {
                nb_brace--;
            }
            if (tokens[i].type == TK_LBRACE) {
                nb_brace++;
            }
        }
        return func;
    }

    std::vector<Function> Function::parse(std::vector<Token>& tokens) {
        std::vector<Function> functions;
        std::vector<std::tuple<size_t, size_t>> tokens_to_erase;
        size_t size = tokens.size();
        for (size_t i = 0; i < size; i++) {
            if ((i == 0 || (tokens[i - 1].type == TK_EOL || tokens[i - 1].type == TK_SEMICOLON)) && tokens[i].type == TK_FUNCTION) {
                size_t start = i;
                Function func = parse_func(tokens, size, i);
                tokens_to_erase.emplace_back(start, i);
                functions.push_back(func);
            }
        }
        std::vector<Token> new_tokens;
        new_tokens.reserve(size);
        for (size_t i = 0; i < size; i++) {
            bool erase = false;
            for (const auto &t: tokens_to_erase) {
                if (i >= std::get<0>(t) && i <= std::get<1>(t)) {
                    erase = true;
                    break;
                }
            }
            if (!erase) {
                new_tokens.push_back(tokens[i]);
            }
        }
        tokens = std::move(new_tokens);
        return functions;
    }
}
