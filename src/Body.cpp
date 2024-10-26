#include "../include/Body.hpp"
#include "../include/Program.hpp"

namespace Lexer {
    std::ostream &operator<<(std::ostream &os, const Body &body)
    {
        size_t i = 0;
        bool ret = false;
        while (body.value[i].type == TK_SEMICOLON)
            i++;
        for (; i < body.value.size(); i++) {
            Types type;

            switch (body.value[i].type) {
            case TK_TRUE_LITERAL:
            case TK_FALSE_LITERAL:
            case TK_NUMBER:
                type = JS_ANY;
                os << TypeNames[type] << "(" << body.value[i].value << ")";
                break;
            case TK_STRING:
                type = JS_ANY;
                os << TypeNames[type] << "(\"" << body.value[i].value << "\")";
                break;
            case TK_NULL_LITERAL:
                type = JS_ANY;
                os << TypeNames[type] << "(nullptr)";
                break;
            case TK_UNDEFINED_LITERAL:
                type = JS_ANY;
                os << TypeNames[type] << "()";
                break;
            case TK_PERIOD:
                os << "::";
                break;
            case TK_CONST:
                os << "const ";
            case TK_LET:
                type =
                    JS_ANY; // TODO: change to check further the type of the variable
                os << TypeNames[type] << " ";
                break;
            case TK_VAR:
                // TODO: add the variable to the list of variables that is outof the scope
                type =
                    JS_ANY; // TODO: change to check further the type of the variable
                os << TypeNames[type] << " ";
                break;
            case TK_RETURN:
                ret = true;
                os << "return ";
                if (i + 1 > body.value.size()) {
                    throw std::runtime_error(
                        "the JavaScript code contain a feature that is not yet implemented.");
                }
                switch (body.value[i + 1].type) {
                case TK_EOL:
                case TK_SEMICOLON:
                    type = JS_ANY;
                    os << TypeNames[type] << "();";
                    break;
                default:
                    break;
                }
                break;
            default:
                if (body.value[i].type != TK_EOL &&
                    body.value[i].type != TK_SEMICOLON &&
                    (i + 1 < body.value.size() && !ret))
                    ret = false;
                os << body.value[i].value;
            }
        }
        if (!ret && body.no_return)
            os << "return JS::Any();";
        return os;
    }
}