#include <iostream>
#include <cstring>
#include "../../include/Tokenizer/Lexer.hpp"

Lexer::Lexer::Lexer(std::string& content)
    : content(content),
      cursor(0),
      line(0),
      start_of_line(0) {
}

Lexer::Lexer::Lexer(const char* content)
    : content(content),
      cursor(0),
      line(0),
      start_of_line(0) {
}

std::vector<Lexer::Token> Lexer::Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!this->content.empty()) {
        Token token = this->nextToken();
        if (token.type == TK_EOS) {
            break;
        }
        tokens.push_back(token);
    }
    return tokens;
}

void Lexer::Lexer::leftTrim() {
    while (this->cursor < this->content.size() &&
           Token::isSkippable(this->content[this->cursor])) {
        this->cursor++;
    }
}

Lexer::Token Lexer::Lexer::nextToken() {
    this->leftTrim();
    if (this->cursor >= this->content.size()) {
        return Token(TK_EOS);
    }
    char ch = this->content[this->cursor];

    // Single line comment
    if (this->cursor + 1 < this->content.size() && ch == '/' && this->content[this->cursor + 1] == '/') {
        this->cursor += 1;
        while (this->cursor < this->content.size() && this->content[this->cursor] != '\n') {
            this->cursor++;
        }
        return this->nextToken();
    }

    // Multi-line comment
    if (this->cursor + 1 < this->content.size() && ch == '/' && this->content[this->cursor + 1] == '*') {
        this->cursor += 2;
        while (this->cursor < this->content.size() && (this->content[this->cursor] != '*' || this->content[this->cursor + 1] != '/')) {
            this->cursor++;
        }
        this->cursor += 2;
        return this->nextToken();
    }

    // Semicolon
    if (ch == ';') {
        this->cursor++;
        return {std::string(1, ch), TK_SEMICOLON, this->line, (this->cursor - this->start_of_line)};
    }

    // End of line
    if (ch == '\n' ||
        (ch == '\r' && this->cursor + 1 < this->content.size() && this->content[this->cursor + 1] == '\n')) {
        this->cursor += (ch == '\r' ? 2 : 1);
        this->line++;
        this->start_of_line = this->cursor;
        while (this->cursor < this->content.size() && (
                   this->content[this->cursor] == '\n' ||
                   (this->content[this->cursor] == '\r' && this->cursor + 1 < this->content.size() && this->content[this->cursor + 1] == '\n'))) {
            this->cursor += (this->content[this->cursor] == '\r' ? 2 : 1);
            this->line++;
            this->start_of_line = this->cursor;
        }
        return {std::string(1, '\n'), TK_EOL, this->line - 1, 0};
    }

    // String literal
    if (ch == '\'' || ch == '"') {
        std::string value;
        size_t backslash = 0;
        this->cursor++;
        while (this->cursor < this->content.size() &&
               (this->content[this->cursor] != ch || backslash % 2 != 0)) {
            if (this->content[this->cursor] == (ch == '\'' ? '"' : '\'')) {
                value += '\\';
            }
            backslash = (this->content[this->cursor] == '\\' ? backslash + 1 : 0);
            value += this->content[this->cursor];
            this->cursor++;
        }
        this->cursor++;
        return {value, TK_STRING, this->line, (this->cursor - this->start_of_line) - value.size()};
    }

    // Identifier or keyword
    if (Token::isSymbolStart(ch)) {
        std::string value;
        while (this->cursor < this->content.size() &&
               Token::isSymbol(this->content[this->cursor])) {
            value += this->content[this->cursor];
            this->cursor++;
        }
        if (Token::isAlpha(value)) {
            TokenType type = Token::isKeyword(value);
            if (type != TK_NOT_FOUND) {
                return {
                    value, type, this->line,
                    (this->cursor - this->start_of_line) - value.size()
                };
            }
            return {value, TK_IDENTIFIER, this->line, (this->cursor - this->start_of_line) - value.size()};
        }
    }

    // Number (integer or float)
    if (isdigit(ch) || (ch == '.' && isdigit(this->content[this->cursor + 1]))) {
        std::string value;
        while ((this->cursor < this->content.size() &&
                isalnum(this->content[this->cursor])) || this->content[this->cursor] == '-' || this->content[this->cursor] == '.' ||
               this->content[this->cursor] == 'e' || this->content[this->cursor] == 'E' || (this->content[this->cursor] == '+' &&
                                                                                            (this->cursor - 1 != 0 && this->content[this->cursor - 1] == 'e' || this->content[this->cursor - 1] == 'E'))) {
            value += this->content[this->cursor];
            this->cursor++;
        }
        return {value, TK_NUMBER, this->line, (this->cursor - this->start_of_line) - value.size()};
    }

    // Regex
    if (ch == '/' && this->cursor + 1 < this->content.size() && this->content[this->cursor + 1] != '/' && this->content[this->cursor + 1] != '*') {
        std::string value;
        size_t backslash = 0;
        this->cursor++;
        value += '/';
        while (this->cursor < this->content.size() &&
               (this->content[this->cursor] != '/' || backslash % 2 != 0)) {
            backslash = (this->content[this->cursor] == '\\' ? backslash + 1 : 0);
            value += this->content[this->cursor];
            this->cursor++;
        }
        this->cursor++;
        value += '/';
        // Regex flags
        while (this->cursor < this->content.size() && isalpha(this->content[this->cursor])) {
            value += this->content[this->cursor];
            this->cursor++;
        }
        return {value, TK_REGEXP, this->line, (this->cursor - this->start_of_line) - value.size()};
    }

    for (size_t i = TK_QUAD_OP_LAST; i > TK_SIMPLE_OP_FIRST; i--) {
        if (TokenValue[i] == nullptr) {
            continue;
        }
        if (this->content.compare(this->cursor, strlen(TokenValue[i]), TokenValue[i]) == 0) {
            this->cursor += strlen(TokenValue[i]);
            return {TokenValue[i], static_cast<TokenType>(i), this->line, (this->cursor - this->start_of_line) - strlen(TokenValue[i])};
        }
    }
    return {};
}