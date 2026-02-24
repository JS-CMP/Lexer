#include "TestUtils.hpp"
#include <algorithm>
#include <cctype>

namespace TestUtils {

std::string processJavaScriptFile(const std::string& filePath) {
    // Handle relative paths by trying to resolve from source directory
    std::string resolvedPath = filePath;
    std::ifstream jsFile(resolvedPath);
    
    if (!jsFile.is_open()) {
        // Try relative to source directory
        resolvedPath = "../" + filePath;
        jsFile.open(resolvedPath);
        
        if (!jsFile.is_open()) {
            throw std::runtime_error("Could not open file: " + filePath + " (tried: " + resolvedPath + ")");
        }
    }
    
    std::stringstream jsBuffer;
    jsBuffer << jsFile.rdbuf();
    std::string jsCode = jsBuffer.str();
    
    // Step 1: Lexing
    Lexer::Lexer lexer(jsCode);
    std::vector<Lexer::Token> tokens = lexer.tokenize();
    
    // Step 2: Parsing
    Lexer::AST::Parser parser(tokens);
    std::vector<Lexer::AST::Stmt::Ptr> ast;
    
    while (!parser.isAtEnd()) {
        auto stmt = parser.parseStatement();
        if (stmt) {
            ast.push_back(std::move(stmt));
        }
    }
    
    // Step 3: Optimization and Transpilation
    Lexer::Opt::Optimizer optimizer(ast);
    optimizer.optimize(0); // No optimization flags for now
    std::string transpiledCode = optimizer.transpile();
    
    return transpiledCode;
}

std::string readExpectedOutput(const std::string& filePath) {
    // Handle relative paths by trying to resolve from source directory
    std::string resolvedPath = filePath;
    std::ifstream outFile(resolvedPath);
    
    if (!outFile.is_open()) {
        // Try relative to source directory
        resolvedPath = "../" + filePath;
        outFile.open(resolvedPath);
        
        if (!outFile.is_open()) {
            throw std::runtime_error("Could not open expected output file: " + filePath + " (tried: " + resolvedPath + ")");
        }
    }
    
    std::stringstream outBuffer;
    outBuffer << outFile.rdbuf();
    return outBuffer.str();
}

std::string normalizeWhitespace(const std::string& str) {
    std::string result;
    bool inWhitespace = false;
    
    for (char c : str) {
        if (std::isspace(c)) {
            if (!inWhitespace) {
                result += ' ';
                inWhitespace = true;
            }
        } else {
            result += c;
            inWhitespace = false;
        }
    }
    
    return result;
}

bool compareOutputs(const std::string& actual, const std::string& expected) {
    std::string normalizedActual = normalizeWhitespace(actual);
    std::string normalizedExpected = normalizeWhitespace(expected);
    
    return normalizedActual == normalizedExpected;
}

std::vector<std::string> findTestFiles(const std::string& directory) {
    std::vector<std::string> testFiles;
    
    // Get the absolute path to handle working directory issues
    std::filesystem::path absPath = std::filesystem::absolute(directory);
    
    if (!std::filesystem::exists(absPath)) {
        // Try relative to source directory
        absPath = std::filesystem::absolute("../" + directory);
    }
    
    if (!std::filesystem::exists(absPath)) {
        throw std::runtime_error("Test directory not found: " + directory);
    }
    
    for (const auto& entry : std::filesystem::recursive_directory_iterator(absPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".js") {
            testFiles.push_back(entry.path().string());
        }
    }
    
    return testFiles;
}

} // namespace TestUtils