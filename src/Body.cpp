#include "../include/Body.hpp"

namespace Lexer {
    std::ostream &operator<<(std::ostream &os, const Body &body) {
        size_t i = 0;
        bool ret = false;
        while (body.value[i].type == TK_EOL) {
            i++;
        }
        size_t size = body.value.size();
        for (; i < size; i++) {
            Types type;

            if (Body::encapsulate(body, size, type, os, i)) {
                continue;
            }
            switch (body.value[i].type) {
                case TK_PERIOD:
                    os << "::";
                    break;
                case TK_CONST:
                    os << "const ";
                    [[fallthrough]];
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
                case TK_EOL:
                    if (i != 0 && (body.value[i - 1].type == TK_SEMICOLON || body.value[i - 1].type == TK_LBRACE)) {
                        continue;
                    }
                    [[fallthrough]];
                case TK_SEMICOLON:
                    os << ";";
                    break;
                case TK_RETURN:
                    ret = true;
                    os << "return ";
                    if (i + 1 > size) {
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
                        (body.value[i].type != TK_SEMICOLON && body.value[i].type != TK_EOL) &&
                        (i + 1 < size && !ret)) {
                        ret = false;
                    }
                    os << body.value[i].value;
            }
        }
        if (!ret && body.no_return) {
            os << "return JS::Any();";
        }
        return os;
    }

    bool Body::encapsulate(const Body &body, size_t size, Types &type, std::ostream &os, size_t &i) {
        switch (body.value[i].type) {
            case TK_TRUE_LITERAL:
            case TK_FALSE_LITERAL:
            case TK_NUMBER:
                type = JS_ANY;
                os << TypeNames[type] << "(" << body.value[i].value << ")";
                return true;
            case TK_STRING:
                type = JS_ANY;
                os << TypeNames[type] << "(\"" << body.value[i].value << "\")";
                return true;
            case TK_NULL_LITERAL:
                type = JS_ANY;
                os << TypeNames[type] << "(nullptr)";
                return true;
            case TK_UNDEFINED_LITERAL:
                type = JS_ANY;
                os << TypeNames[type] << "()";
                return true;
            default:
                break;
        }
        return Body::transpileObject(body, size, type, os, i);
    }

    bool Body::transpileObject(const Body &body, size_t size, Types &type, std::ostream &os, size_t &i) {
        size_t base_i = i;
        size_t start = i;
        if (body.value[i].type != TK_LBRACE) {
            return false;
        }
        i++;
        i = eraseEol(body, size, i);
        if (body.value[i].type == TK_RBRACE) {
            os << TypeNames[type] << "(JS::Object())"; //TODO: remove JS::Object if possible
            return true;
        }
        if (body.value[i].type != TK_STRING && body.value[i + 1].type != TK_COLON) {
            i = base_i;
            return false;
        }
        start = i;
        i++;
        i = eraseEol(body, size, i);
        if (body.value[i].type != TK_COLON) {
            i = base_i;
            return false;
        }
        type = JS_ANY;
        os << TypeNames[type] << "(" << "JS::Object({"; //TODO: remove JS::Object if possible
        // example of object
        // { "key": "value", "key2": 2 }
        // example of transpiled object
        // JS::Object({{"key", "value"}, {"key2", 2}})
        i = start - 1;
        while (i + 1 < size && body.value[i].type != TK_RBRACE) {
            os << "{";
            i = eraseEol(body, size, i);
            switch (body.value[i].type) {
                case TK_STRING:
                    os << "\"" << body.value[i].value << "\"";
                    break;
                case TK_IDENTIFIER:
                    os << "\"" << body.value[i].value << "\"";
                    break;
                default:
                    return false;
            }
            i++;
            i = eraseEol(body, size, i);
            if (i < size && body.value[i].type != TK_COLON) {
                return false;
            }
            os << ", ";
            i++;
            i = eraseEol(body, size, i);
            if (i < size && body.value[i].type == TK_IDENTIFIER) {
                os << body.value[i].value;
            } else if (i < size && !encapsulate(body, size, type, os, i)) {
                return false;
            }
            os << "}";
            i++;
            i = eraseEol(body, size, i);
            if (i < size && body.value[i].type == TK_COMMA) {
                os << ",";
                i++;
            }
            i = eraseEol(body, size, i);
        }
        os << "})";
        return true;
    }

    size_t Body::eraseEol(const Body &body, size_t size, size_t &i) {
        while (i < size && body.value[i].type == TK_EOL) i++;
        return i;
    }
}