#include <Lexer/Tokenizer/Lexer.hpp>
#include <iostream>
#include <unordered_map>
#include <algorithm>

/**
 * Token Analysis Example
 * 
 * This example demonstrates advanced token analysis including:
 * - Token frequency counting
 * - Identifier extraction
 * - Code complexity metrics
 * - Keyword usage analysis
 */

class CodeAnalyzer {
private:
    std::unordered_map<std::string, int> identifierCounts;
    std::unordered_map<std::string, int> keywordCounts;
    std::unordered_map<std::string, int> operatorCounts;
    int totalTokens = 0;
    int totalLines = 0;
    int maxLineLength = 0;
    
public:
    void analyze(const std::string& code) {
        try {
            Lexer::Lexer lexer(code);
            std::vector<Lexer::Token> tokens = lexer.tokenize();
            
            analyzeTokens(tokens);
            
        } catch (const std::exception& e) {
            std::cerr << "Analysis error: " << e.what() << std::endl;
        }
    }
    
    void analyzeTokens(const std::vector<Lexer::Token>& tokens) {
        for (const auto& token : tokens) {
            totalTokens++;
            
            // Track line metrics
            if (token.line > totalLines) {
                totalLines = token.line;
            }
            
            // Count identifiers
            if (token.type == Lexer::TK_IDENTIFIER) {
                identifierCounts[token.value]++;
            }
            
            // Count keywords (range from TK_DELETE to TK_VAR)
            if (token.type >= Lexer::TK_DELETE && token.type <= Lexer::TK_VAR) {
                std::string keywordName = Lexer::TokenName[token.type];
                keywordCounts[keywordName]++;
            }
            
            // Count operators
            if (isOperatorToken(token.type)) {
                std::string opName = Lexer::TokenName[token.type];
                operatorCounts[opName]++;
            }
        }
    }
    
    bool isOperatorToken(Lexer::TokenType type) {
        // Check if token is in operator ranges
        return (type >= Lexer::TK_SIMPLE_OP_FIRST && type <= Lexer::TK_SIMPLE_OP_LAST) ||
               (type >= Lexer::TK_DOUBLE_OP_FIRST && type <= Lexer::TK_DOUBLE_OP_LAST) ||
               (type >= Lexer::TK_TRIPLE_OP_FIRST && type <= Lexer::TK_TRIPLE_OP_LAST) ||
               (type >= Lexer::TK_QUAD_OP_FIRST && type <= Lexer::TK_QUAD_OP_LAST);
    }
    
    void printReport() const {
        std::cout << "=== Code Analysis Report ===" << std::endl;
        std::cout << std::string(50, '=') << std::endl << std::endl;
        
        std::cout << "📊 General Statistics:" << std::endl;
        std::cout << "  Total tokens: " << totalTokens << std::endl;
        std::cout << "  Total lines: " << totalLines << std::endl;
        std::cout << "  Unique identifiers: " << identifierCounts.size() << std::endl;
        std::cout << "  Unique keywords: " << keywordCounts.size() << std::endl;
        std::cout << "  Unique operators: " << operatorCounts.size() << std::endl;
        std::cout << std::endl;
        
        if (!identifierCounts.empty()) {
            std::cout << "🔤 Top 5 Identifiers:" << std::endl;
            printTopItems(identifierCounts, 5);
            std::cout << std::endl;
        }
        
        if (!keywordCounts.empty()) {
            std::cout << "🔑 Keyword Usage:" << std::endl;
            printTopItems(keywordCounts, 10);
            std::cout << std::endl;
        }
        
        if (!operatorCounts.empty()) {
            std::cout << "⚙️  Operator Usage:" << std::endl;
            printTopItems(operatorCounts, 10);
            std::cout << std::endl;
        }
        
        // Calculate complexity metrics
        calculateComplexity();
    }
    
    void printTopItems(const std::unordered_map<std::string, int>& items, int limit) const {
        // Convert to vector and sort by count
        std::vector<std::pair<std::string, int>> sortedItems(items.begin(), items.end());
        std::sort(sortedItems.begin(), sortedItems.end(),
                 [](const auto& a, const auto& b) { return a.second > b.second; });
        
        // Print top items
        int count = 0;
        for (const auto& [name, count] : sortedItems) {
            if (count >= limit) break;
            std::cout << "    " << name << ": " << count << " occurrences" << std::endl;
            count++;
        }
    }
    
    void calculateComplexity() const {
        std::cout << "📈 Complexity Metrics:" << std::endl;
        
        // Cyclomatic complexity estimate
        int complexity = keywordCounts.at("TK_IF") + 
                        keywordCounts.at("TK_FOR") + 
                        keywordCounts.at("TK_WHILE") + 
                        keywordCounts.at("TK_SWITCH") + 
                        keywordCounts.at("TK_CASE");
        
        std::cout << "  Estimated cyclomatic complexity: " << complexity << std::endl;
        
        // Token density
        if (totalLines > 0) {
            double density = static_cast<double>(totalTokens) / totalLines;
            std::cout << "  Tokens per line: " << std::fixed << std::setprecision(2) << density << std::endl;
        }
        
        std::cout << std::endl;
    }
};

int main() {
    // Complex JavaScript code for analysis
    std::string complexCode = R"(
        // Complex function with multiple control structures
        function analyzeData(data, threshold) {
            let results = [];
            let total = 0;
            
            for (let i = 0; i < data.length; i++) {
                const item = data[i];
                
                if (item.value > threshold) {
                    results.push(item);
                    total += item.value;
                } else if (item.value < threshold / 2) {
                    console.warn('Low value detected:', item.value);
                }
                
                switch (item.type) {
                    case 'A':
                        item.category = 'High';
                        break;
                    case 'B':
                        item.category = 'Medium';
                        break;
                    default:
                        item.category = 'Low';
                }
            }
            
            return {
                filtered: results,
                total: total,
                average: total / results.length
            };
        }
        
        // Usage example
        const testData = [
            {value: 100, type: 'A'},
            {value: 50, type: 'B'},
            {value: 25, type: 'C'},
            {value: 200, type: 'A'}
        ];
        
        const analysis = analyzeData(testData, 75);
        console.log('Analysis complete:', analysis);
    )"
    ";

    std::cout << "=== Token Analysis Example ===" << std::endl;
    std::cout << "Analyzing complex JavaScript code..." << std::endl << std::endl;

    // Create and run analyzer
    CodeAnalyzer analyzer;
    analyzer.analyze(complexCode);
    analyzer.printReport();

    return 0;
}