#include "../include/Program.hpp"

Lexer::Program::Program(std::vector<Token> &tokens)
    : tokens(tokens)
{
    // TODO make this dynamic to include only the necessary files
    this->includes.emplace_back("types/JsAny.hpp");
    this->includes.emplace_back("global/global.hpp");
    this->includes.emplace_back("types/objects/Types.hpp");
    this->includes.emplace_back("customOperators/CustomOperators.hpp");
}

void Lexer::Program::parse()
{
    Body body;
    body.value = this->tokens;
    body.parse();
    this->main = body;
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
        os << "int main() {";
        os << program.main;
        os << "}" << std::endl;
        return os;
    }
}

