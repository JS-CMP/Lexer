#include "../include/Body.hpp"

#include <sstream>

#include "Function.hpp"

namespace Lexer {
    bool push_tokens(const Body& body, std::ostringstream &body_os, size_t& i, size_t size, std::ostringstream *vars = nullptr) {
        if (Body::encapsulate(body, size, body_os, i)) {
            return true;
        }
        switch (body.value[i].type) {
            case TK_ASSIGN_ADD:
            case TK_ASSIGN_SUB:
            case TK_ASSIGN_MUL:
            case TK_ASSIGN_MOD:
            case TK_ASSIGN_DIV:
            case TK_ASSIGN_BIT_AND:
            case TK_ASSIGN_BIT_OR:
            case TK_ASSIGN_BIT_XOR:
            case TK_ASSIGN_SAR:
            case TK_ASSIGN_SHL:
                if (body.value[i - 1].type == TK_IDENTIFIER) {
                    body_os << " = " << body.value[i - 1].value << " " << body.value[i].value.substr(0, body.value[i].value.size() - 1);
                } else {
                    throw std::runtime_error("Invalid assignment operator usage.");
                }
                break;
            case TK_ASSIGN_SHR:
                if (body.value[i - 1].type == TK_IDENTIFIER) {
                    body_os << " = " << body.value[i - 1].value << " URightShift ";
                } else {
                    throw std::runtime_error("Invalid assignment operator usage.");
                }
                break;
            case TK_SHR:
                body_os << " URightShift ";
                break;
            case TK_THIS:
                body_os << "thisArg";
                break;
            case TK_EQ_STRICT:
                body_os << " strictEq ";
                break;
            case TK_NE_STRICT:
                body_os << " strictNeq ";
                break;
            case TK_PERIOD:
                if (i + 1 < size && body.value[i + 1].type == TK_IDENTIFIER) {
                    body_os << "[\"" << body.value[i + 1].value << "\"]";
                    i++;
                }
                break;
            case TK_VAR:
                if (vars) {
                    (*vars) << body.value[i + 1].value << ", ";
                } else {
                    body_os << "JS::Any " << body.value[i + 1].value;
                    i++;
                }
                break;
            case TK_EOL:
                if (i != 0 && (body.value[i - 1].type == TK_SEMICOLON || body.value[i - 1].type == TK_LBRACE)) {
                    return true;
                }
                [[fallthrough]];
            case TK_SEMICOLON:
                body_os << ";";
                break;
            case TK_RETURN:
                body_os << "return ";
                if (i + 1 > size) {
                    throw std::runtime_error(
                        "the JavaScript code contain a feature that is not yet implemented.");
                }
                switch (body.value[i + 1].type) {
                    case TK_EOL:
                    case TK_SEMICOLON:
                        body_os << TypeNames[JS_ANY] << "();";
                        break;
                    default:
                        break;
                }
                break;
            default:
                body_os << body.value[i].value;
        }
        return false;
    }

    std::ostream &operator<<(std::ostream &os, const Body &body) {
        std::ostringstream vars;
        std::ostringstream body_os;

        size_t function_size = body.functions.size();
        if (function_size != 0) {
            for (size_t i = 0; i < function_size; i++) {
                vars << body.functions[i].name << ", ";
            }

            for (const auto &function : body.functions) {
                body_os << function.name << " = " << function << ";";
            }
        }


        size_t i = 0;
        size_t size = body.value.size();

        Body::eraseEol(body, size, i);
        for (; i < size; i++) {
            if (push_tokens(body, body_os, i, size, &vars)) continue;
        }
        if (size > 0 && body.value[size - 1].type != TK_SEMICOLON && body.value[size - 1].type != TK_EOL) {
            body_os << ";";
        }
        std::string str = vars.str();
        if (str.size() >= 2) {
            str.erase(str.size() - 2);
            os << "JS::Any " << str << ";" << body_os.str();
        } else {
            os << body_os.str();
        }
        return os;
    }

    Body::Body(const std::vector<Token>& tokens) : value(tokens) {
        this->parse();
    }

    void Body::parse() {
        this->functions = Function::parse(this->value);
    }

    bool Body::encapsulate(const Body &body, size_t size, std::ostringstream &os, size_t &i) {
        switch (body.value[i].type) {
            case TK_TRUE_LITERAL:
            case TK_FALSE_LITERAL:
            case TK_NUMBER:
                os << TypeNames[JS_ANY] << "(" << body.value[i].value << ")";
                return true;
            case TK_STRING:
                os << TypeNames[JS_ANY] << "(\"" << body.value[i].value << "\")";
                return true;
            case TK_NULL_LITERAL:
                os << TypeNames[JS_ANY] << "(nullptr)";
                return true;
            case TK_UNDEFINED_LITERAL:
                os << TypeNames[JS_ANY] << "()";
                return true;
            case TK_NEW:
                return Body::transpileNew(body, size, os, i);
            case TK_LBRACE:
                return Body::transpileObject(body, size, os, i);
            case TK_LBRACK:
                return Body::transpileArray(body, size, os, i);
            case TK_FUNCTION:
                os << Function::parse_func(body.value, size, i);
                return true;
            case TK_ELSE:
                os << "else ";
                if (i + 1 < size && body.value[i + 1].type == TK_IF) {
                    i++;
                } else {
                    return Body::transpileBlocks(body, size, os, i, {
                        {TK_LBRACE, TK_RBRACE, true},
                    });
                }
                [[fallthrough]];
            case TK_IF:
                os << "if ";
                return Body::transpileBlocks(body, size, os, i, {
                    {TK_LPAREN, TK_RPAREN, false},
                    {TK_LBRACE, TK_RBRACE, true},
                });
            case TK_WHILE:
                os << "while ";
                return Body::transpileBlocks(body, size, os, i, {
                {TK_LPAREN, TK_RPAREN, false},
                    {TK_LBRACE, TK_RBRACE, true},
                });
            case TK_DO:
                os << "do ";
                if (!Body::transpileBlocks(body, size, os, i, {
                    {TK_LBRACE, TK_RBRACE, true},
                })) {
                    return false;
                }
                i++;
                i = eraseEol(body, size, i);
                if (body.value[i].type != TK_WHILE) {
                    return false;
                }
                os << "while ";
                return Body::transpileBlocks(body, size, os, i, {
                    {TK_LPAREN, TK_RPAREN, false},
                });
            case TK_FOR:
                os << "for ";
                return Body::transpileBlocks(body, size, os, i, {
                    {TK_LPAREN, TK_RPAREN, false},
                    {TK_LBRACE, TK_RBRACE, true},
                });
            default:
                return false;
        }
    }
    bool Body::transpileNew(const Body &body, size_t size, std::ostringstream &os, size_t &i) {
        if (body.value[i].type != TK_NEW) {
            return false;
        }

        os << "NEW(";
        i++;
        i = eraseEol(body, size, i);
        if (body.value[i].type == TK_NEW) {
            Body::encapsulate(body, size, os, i);
            i++;
            i = eraseEol(body, size, i);
            while (i + 1 < size && body.value[i].type == TK_PERIOD && body.value[i + 1].type == TK_IDENTIFIER) {
                push_tokens(body, os, i, size);
                i++;
            }
        } else if (body.value[i].type == TK_IDENTIFIER) {
            push_tokens(body, os, i, size);
            i++;
            while (i + 1 < size && body.value[i].type == TK_PERIOD &&
                   body.value[i + 1].type == TK_IDENTIFIER) {
                push_tokens(body, os, i, size);
                i++; // Skip the period and identifier
            }
        } else if (body.value[i].type == TK_LPAREN) {
            size_t nb_parens = 1;
            i++;
            i = eraseEol(body, size, i);
            os << "(";
            while (i < size && nb_parens > 0) {
                if (body.value[i].type == TK_LPAREN) {
                    nb_parens++;
                } else if (body.value[i].type == TK_RPAREN) {
                    nb_parens--;
                }
                push_tokens(body, os, i, size);
                i++;
            }
        }
        i = eraseEol(body, size, i);
        if (body.value[i].type != TK_LPAREN) {
            os << ")";
            i--; // Move back to the last token
            return true;
        }
        if (body.value[i].type == TK_LPAREN && i + 1 < size && body.value[i + 1].type == TK_RPAREN) {
            os << ")";
            i += 1; // Skip the parentheses
            i = eraseEol(body, size, i);
            return true;
        }
        os << ",";
        i++;
        i = eraseEol(body, size, i);
        size_t nb_parens = 1;
        while (i < size && nb_parens > 0) {
            if (body.value[i].type == TK_LPAREN) {
                nb_parens++;
            } else if (body.value[i].type == TK_RPAREN) {
                nb_parens--;
            }
            if (nb_parens > 0) {
                push_tokens(body, os, i, size);
            }
            i++;
        }
        i--;
        os << ")";
        return true;
    }

    bool Body::transpileBlocks(const Body &body, size_t size, std::ostringstream &os, size_t &i, const std::vector<std::tuple<TokenType, TokenType, bool>> &stack) {
        for (const auto &tuple : stack) {
            TokenType first = std::get<0>(tuple);
            TokenType second = std::get<1>(tuple);
            bool bodyType = std::get<2>(tuple);

            i++;
            i = eraseEol(body, size, i);
            if (body.value[i].type != first) {
                return false;
            }
            int nb_brace = 1;
            std::vector<Token> body_tokens;

            os << TokenValue[first];
            i++;
            i = eraseEol(body, size, i);
            if (body.value[i].type == second) {
                nb_brace--;
            }
            while (nb_brace != 0 && i < size) {
                if (bodyType) {
                    body_tokens.push_back(body.value[i]);
                } else {
                    push_tokens(body, os, i, size);
                }
                i++;
                if (body.value[i].type == first) {
                    nb_brace++;
                }
                if (body.value[i].type == second) {
                    nb_brace--;
                }
            }
            if (bodyType) {
                os << Body(body_tokens);
            }
            os << TokenValue[second];
        }
        return true;
    }

    bool Body::transpileObject(const Body &body, size_t size, std::ostringstream &os, size_t &i) {
        size_t base_i = i;
        size_t start = i;
        if (body.value[i].type != TK_LBRACE) {
            return false;
        }
        i++;
        i = eraseEol(body, size, i);
        if (body.value[i].type == TK_RBRACE) {
            os << TypeNames[JS_ANY] << "(JS::Object())"; //TODO: remove JS::Object if possible
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
        os << TypeNames[JS_ANY] << "(JS::Object({"; //TODO: remove JS::Object if possible
        // example of object
        // { "key": "value", "key2": 2 }
        // example of transpiled object
        // JS::Object({{"key", "value"}, {"key2", 2}})
        i = start;
        while (i + 1 < size && body.value[i].type != TK_RBRACE) {
            os << "{";
            i = eraseEol(body, size, i);
            switch (body.value[i].type) {
                case TK_STRING:
                case TK_IDENTIFIER:
                    os << "u\"" << body.value[i].value << "\"";
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
            } else if (i < size && !encapsulate(body, size, os, i)) {
                return false;
            }
            os << "}";
            i++;
            i = eraseEol(body, size, i);
            if (i < size && body.value[i].type == TK_COMMA) {
                os << ",";
                i++;
                i = eraseEol(body, size, i);
            }
        }
        os << "}))";
        return true;
    }

    bool Body::transpileArray(const Body &body, size_t size, std::ostringstream &os, size_t &i) {
        if (body.value[i].type != TK_LBRACK ||
            (size > 0 && (body.value[i - 1].type == TK_IDENTIFIER ||
            body.value[i - 1].type == TK_STRING || body.value[i - 1].type == TK_RPAREN))) {
            return false;
        }
        os << TypeNames[JS_ANY] << "(" << "JS::Array({";
        i++;
        i = eraseEol(body, size, i);
        while (i < size && body.value[i].type != TK_RBRACK) {
            if (i < size && body.value[i].type == TK_IDENTIFIER) {
                os << body.value[i].value;
            } else if (i < size && !encapsulate(body, size, os, i)) {
                return false;
            }
            i = eraseEol(body, size, i);
            i++;
            if (i < size && body.value[i].type == TK_COMMA) {
                os << ", ";
                i++;
            }
            i = eraseEol(body, size, i);
        }
        os << "}))";
        return true;
    }


    size_t Body::eraseEol(const Body &body, size_t size, size_t &i) {
        while (i < size && body.value[i].type == TK_EOL) i++;
        return i;
    }
}
