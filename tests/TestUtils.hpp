#ifndef JS_CMP_LEXER_TEST_UTILS_HPP
#define JS_CMP_LEXER_TEST_UTILS_HPP

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "Tokenizer/Lexer.hpp"
#include "AST/Parser.hpp"
#include "Optimization/Optimizer.hpp"

namespace TestUtils {

/**
 * Process a JavaScript file through the full pipeline (lexing -> parsing -> transpiling)
 * and return the transpiled C++ code.
 */
std::string processJavaScriptFile(const std::string& filePath);

/**
 * Read the expected output from a .out file.
 */
std::string readExpectedOutput(const std::string& filePath);

/**
 * Compare the actual output with the expected output, ignoring whitespace differences.
 */
bool compareOutputs(const std::string& actual, const std::string& expected);

/**
 * Find all JavaScript test files in the given directory.
 */
std::vector<std::string> findTestFiles(const std::string& directory);

/**
 * Normalize whitespace in a string for comparison.
 */
std::string normalizeWhitespace(const std::string& str);

} // namespace TestUtils

#endif // JS_CMP_LEXER_TEST_UTILS_HPP