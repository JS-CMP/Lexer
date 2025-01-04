#include "../include/Program.hpp"

Lexer::Program::Program(std::vector<Token> &tokens)
    : tokens(tokens)
{
    // TODO make this dynamic to include only the necessary files
    this->includes.emplace_back("types/JsAny.hpp");
    this->includes.emplace_back("class/Console.hpp");
    this->includes.emplace_back("class/Assert.hpp");
}

void Lexer::Program::parse()
{
    Function main;
    main.name = "main";
    main.return_type = INT;
    main.body.no_return = true;
    for (size_t i = 0; i < this->tokens.size(); i++) {
        if (this->tokens[i].type == TK_FUNCTION) {
            Function func;
            size_t nb_brace = 1;
            parse_func(i, func);
            while (nb_brace != 0) {
                func.body.value.push_back(this->tokens[i]);
                i++;
                if (this->tokens[i].type == TK_RBRACE) {
                    nb_brace--;
                }
                if (this->tokens[i].type == TK_LBRACE) {
                    nb_brace++;
                }
            }
            this->functions.push_back(func);
        } else {
            main.body.value.push_back(this->tokens[i]);
        }
    }
    this->functions.push_back(main);
}

void Lexer::Program::parse_func(size_t &i, Lexer::Function &func)
{
    func.name = tokens[i + 1].value;
    func.return_type = JS_ANY;
    i += 2;
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
}

void Lexer::Program::function_declaration(
    std::ostream &os, const Lexer::Program &program
)
{
    if (program.functions.size() <= 1) {
        return;
    }
    for (const auto &function: program.functions) {
        if (function.name == "main") {
            continue;
        }
        os << TypeNames[function.return_type] << " " << function.name << "(";
        for (size_t i = 0; i < function.args.size(); i++) {
            os << TypeNames[function.args[i].type];
            if (i + 1 < function.args.size()) {
                os << ", ";
            }
        }
        os << ");" << std::endl;
    }
    os << std::endl;
}

void Lexer::Program::include(
    std::ostream &os, const Lexer::Program &program
)
{
    for (const auto &include: program.includes) {
        os << include;
    }
    os << std::endl;
}

namespace Lexer {
    std::ostream &operator<<(std::ostream &os, const Program &program)
    {
        Program::include(os, program);
        Program::function_declaration(os, program);

        Function main = program.functions.back();
        for (const auto &function: program.functions) {
            if (function.name == "main") {
                continue;
            }
            os << function;
        }
        os << main;
        return os;
    }
}

