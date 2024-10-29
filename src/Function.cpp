#include "../include/Function.hpp"

namespace Lexer {
    std::ostream &operator<<(
        std::ostream &os, const Function &function
    )
    {
        os << TypeNames[function.return_type] << " " << function.name << "(";
        for (size_t i = 0; i < function.args.size(); i++) {
            os << TypeNames[function.args[i].type] << " "
                << function.args[i].name;
            if (i < function.args.size() - 1) {
                os << ", ";
            }
        }
        os << ") {";
        os << function.body;
        os << "}" << std::endl;
        return os;
    }
}
