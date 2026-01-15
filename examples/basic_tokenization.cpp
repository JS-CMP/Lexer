#include <Lexer/Tokenizer/Lexer.hpp>
#include <iostream>
#include <vector>

/**
 * Basic Tokenization Example
 * 
 * This example demonstrates how to perform basic tokenization of JavaScript code
 * using the JS-CMP Lexer SDK in batch mode.
 */

int main() {
    // Sample JavaScript code to tokenize
    std::string javascriptCode = R"(
        function greet(name) {
            return "Hello, " + name + "!";
        }
        
        const result = greet("World");
        console.log(result);
    )"
    ";

    std::cout << "=== Basic Tokenization Example ===" << std::endl;
    std::cout << "Input JavaScript code:" << std::endl;
    std::cout << javascriptCode << std::endl << std::endl;

    try {
        // Create lexer instance with the JavaScript code
        Lexer::Lexer lexer(javascriptCode);

        // Tokenize the entire code in batch mode
        std::vector<Lexer::Token> tokens = lexer.tokenize();

        std::cout << "Tokenization results:" << std::endl;
        std::cout << "Total tokens: " << tokens.size() << std::endl << std::endl;

        // Display each token
        for (size_t i = 0; i < tokens.size(); i++) {
            const Lexer::Token& token = tokens[i];
            
            std::cout << "Token " << (i + 1) << ":" << std::endl;
            std::cout << "  Type: " << Lexer::TokenName[token.type] << " (" << token.type << ")" << std::endl;
            std::cout << "  Value: \"" << token.value << "\"" << std::endl;
            std::cout << "  Position: Line " << token.line << ", Column " << token.column << std::endl;
            
            // Add separator between tokens
            if (i < tokens.size() - 1) {
                std::cout << "  " << std::string(50, '-') << std::endl;
            }
        }

        std::cout << std::endl << "Tokenization completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error during tokenization: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}