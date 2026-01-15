#include <Lexer/Tokenizer/Lexer.hpp>
#include <iostream>
#include <iomanip>

/**
 * Streaming Tokenization Example
 * 
 * This example demonstrates how to use the streaming tokenization API
 * for memory-efficient processing of large JavaScript files.
 */

int main() {
    // Sample JavaScript code to tokenize
    std::string javascriptCode = R"(
        // Calculate factorial
        function factorial(n) {
            if (n <= 1) {
                return 1;
            }
            return n * factorial(n - 1);
        }
        
        const result = factorial(5);
        console.log("Factorial of 5 is: " + result);
    )"
    ";

    std::cout << "=== Streaming Tokenization Example ===" << std::endl;
    std::cout << "Processing code in streaming mode..." << std::endl << std::endl;

    try {
        // Create lexer instance
        Lexer::Lexer lexer(javascriptCode);

        std::cout << std::left << std::setw(20) << "Token Type"
                  << std::setw(30) << "Value"
                  << std::setw(15) << "Line"
                  << std::setw(15) << "Column"
                  << std::endl;
        std::cout << std::string(80, '-') << std::endl;

        int tokenCount = 0;
        
        // Process tokens one by one in streaming mode
        while (true) {
            Lexer::Token token = lexer.nextToken();
            
            // Check for end of stream
            if (token.type == Lexer::TK_EOS) {
                break;
            }

            // Skip whitespace and comments for cleaner output
            if (token.type == Lexer::TK_EOL ||
                token.type == Lexer::TK_SINGLE_LINE_COMMENT ||
                token.type == Lexer::TK_MULTI_LINE_COMMENT) {
                continue;
            }

            tokenCount++;
            
            // Display token information
            std::cout << std::left << std::setw(20) << Lexer::TokenName[token.type]
                      << std::setw(30) << (token.value.empty() ? "" : token.value)
                      << std::setw(15) << token.line
                      << std::setw(15) << token.column
                      << std::endl;
        }

        std::cout << std::string(80, '-') << std::endl;
        std::cout << "Streaming tokenization completed!" << std::endl;
        std::cout << "Total tokens processed: " << tokenCount << std::endl;
        std::cout << "Memory efficient: Only one token in memory at a time" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error during streaming tokenization: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}