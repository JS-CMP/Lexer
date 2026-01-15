# JS-CMP Lexer SDK

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-3.25%2B-blue.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ECMAScript](https://img.shields.io/badge/ECMAScript-5-orange.svg)](https://www.ecma-international.org/ecma-262/5.1/)

## 🚀 JavaScript Tokenizer SDK for C++

**JS-CMP Lexer** is a professional-grade JavaScript tokenizer SDK that converts ECMAScript 5 code into structured tokens for analysis, transformation, and compilation. Built in modern C++23, this SDK provides a robust foundation for JavaScript processing tools, transpilers, and code analysis applications.

## 📦 Features

- **ECMAScript 5 Compliance**: Full support for ES5 syntax and semantics
- **High Performance**: Optimized C++23 implementation for fast tokenization
- **Comprehensive Token Types**: 100+ token types covering all JavaScript constructs
- **Flexible APIs**: Streaming and batch tokenization modes
- **Error Handling**: Robust error detection and reporting
- **Modular Design**: Clean separation of lexer, tokenizer, and AST components
- **Cross-Platform**: Works on Linux, macOS, and Windows
- **Header-Only Option**: Available as single-header library

## 🎯 Use Cases

Perfect for building:
- JavaScript transpilers and compilers
- Code analysis and linting tools
- Static analysis engines
- Code formatting and beautification tools
- JavaScript interpreters
- Educational programming tools
- Custom scripting engines

## 📥 Installation

### As GitHub Submodule (Recommended)

```bash
# Add as submodule to your project
git submodule add https://github.com/JS-CMP/Lexer.git extern/lexer
git submodule update --init --recursive

# In your CMakeLists.txt
add_subdirectory(extern/lexer)
target_link_libraries(your_project PRIVATE JS_CMP_LEXER)
```

### Standalone Build

```bash
# Clone the repository
git clone https://github.com/JS-CMP/Lexer.git
cd Lexer

# Build the SDK
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Install (optional)
cmake --install build
```

### Header-Only Usage

```bash
# Copy the include directory to your project
cp -r include/ your_project/include/lexer

# Include in your code
#include "lexer/Tokenizer/Lexer.hpp"
```

## 🚀 Quick Start

### Basic Tokenization

```cpp
#include <Lexer/Tokenizer/Lexer.hpp>
#include <iostream>

int main() {
    // Create lexer instance
    Lexer::Lexer lexer;
    
    // Tokenize JavaScript code
    std::string javascriptCode = "function hello() { return 'world'; }";
    std::vector<Lexer::Token> tokens = lexer.tokenize(javascriptCode);
    
    // Process tokens
    for (const auto& token : tokens) {
        std::cout << "Token: " << Lexer::Token::tokenTypeToString(token.type)
                  << " (" << token.value << ")\n";
    }
    
    return 0;
}
```

### Streaming Tokenization

```cpp
#include <Lexer/Tokenizer/Lexer.hpp>
#include <iostream>

int main() {
    Lexer::Lexer lexer;
    std::string code = "const x = 42 + 'hello';";
    
    // Initialize lexer with code
    lexer.init(code);
    
    // Process tokens one by one
    while (true) {
        Lexer::Token token = lexer.nextToken();
        if (token.type == Lexer::TokenType::END_OF_FILE) {
            break;
        }
        
        std::cout << "Token: " << Lexer::Token::tokenTypeToString(token.type)
                  << " at line " << token.line << ":" << token.column
                  << " (" << token.value << ")\n";
    }
    
    return 0;
}
```

## 🔧 API Reference

### Core Classes

#### `Lexer::Lexer`

The main lexer class providing tokenization functionality.

**Key Methods:**

- `std::vector<Token> tokenize(const std::string& code)` - Batch tokenization
- `void init(const std::string& code)` - Initialize for streaming
- `Token nextToken()` - Get next token in stream
- `void reset()` - Reset lexer state

**Example:**
```cpp
Lexer::Lexer lexer;
std::vector<Lexer::Token> tokens = lexer.tokenize("var x = 5;");
```

#### `Lexer::Token`

Represents a single JavaScript token.

**Properties:**
- `TokenType type` - Token classification
- `std::string value` - Token content
- `size_t line` - Line number (1-based)
- `size_t column` - Column number (1-based)
- `size_t position` - Absolute position in source

**Token Types:**
- Keywords: `FUNCTION`, `RETURN`, `IF`, `ELSE`, etc.
- Literals: `NUMBER`, `STRING`, `BOOLEAN`, `NULL`
- Operators: `PLUS`, `MINUS`, `EQUAL`, `NOT_EQUAL`, etc.
- Punctuation: `LEFT_PAREN`, `RIGHT_PAREN`, `SEMICOLON`, etc.
- Identifiers: `IDENTIFIER`
- Special: `END_OF_FILE`, `UNKNOWN`

**Example:**
```cpp
Lexer::Token token = lexer.nextToken();
if (token.type == Lexer::TokenType::IDENTIFIER) {
    std::cout << "Found identifier: " << token.value;
}
```

## 📚 Advanced Usage

### Error Handling

```cpp
#include <Lexer/Tokenizer/Lexer.hpp>
#include <stdexcept>

int main() {
    Lexer::Lexer lexer;
    
    try {
        std::string invalidCode = "function @test() {}";
        auto tokens = lexer.tokenize(invalidCode);
    } catch (const std::runtime_error& e) {
        std::cerr << "Lexer error: " << e.what() << std::endl;
        // Handle error appropriately
    }
    
    return 0;
}
```

### Custom Token Processing

```cpp
#include <Lexer/Tokenizer/Lexer.hpp>
#include <unordered_map>

class TokenProcessor {
public:
    void process(const std::string& code) {
        Lexer::Lexer lexer;
        auto tokens = lexer.tokenize(code);
        
        std::unordered_map<std::string, int> identifierCount;
        
        for (const auto& token : tokens) {
            if (token.type == Lexer::TokenType::IDENTIFIER) {
                identifierCount[token.value]++;
            }
        }
        
        // Output identifier statistics
        for (const auto& [name, count] : identifierCount) {
            std::cout << name << ": " << count << " occurrences\n";
        }
    }
};
```

## 🧪 Testing

The SDK includes comprehensive unit tests using Google Test framework.

### Running Tests

```bash
# Build and run tests
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/JS_CMP_LEXER_test
```

### Test Coverage

- ✅ Basic syntax (variables, functions, loops)
- ✅ Complex expressions and operators
- ✅ Object and array literals
- ✅ Regular expressions
- ✅ Error conditions and edge cases
- ✅ Performance benchmarks
- ✅ **Advanced Coverage Tracking**: Automatic test execution tracking
- ✅ **Multiple Report Formats**: Console, Markdown, HTML, XML reports
- ✅ **File-Level Coverage**: Detailed file-by-file test results

### Coverage Reporting

The lexer now includes comprehensive test coverage reporting:

```bash
# Build with coverage support
cmake -DENABLE_COVERAGE=ON -B build
cmake --build build

# Run tests with coverage reporting
./build/JS_CMP_LEXER_test
```

This generates detailed coverage reports in `build/reports/` including:
- Console summary with test statistics
- Markdown report with detailed results
- HTML coverage report (when lcov/gcovr installed)
- XML report for CI integration

## 📊 Performance

### Benchmark Results

```
Tokenization Speed: ~100,000 tokens/second
Memory Usage: ~50KB per lexer instance
Startup Time: <1ms
```

### Optimization Tips

1. **Reuse Lexer Instances**: Create lexer once, reuse for multiple files
2. **Batch Processing**: Use `tokenize()` for large files
3. **Streaming Mode**: Use `nextToken()` for memory-constrained environments
4. **Pre-allocation**: Reserve vector space when expecting many tokens

## 🔧 Integration Guide

### CMake Integration

```cmake
# In your CMakeLists.txt
find_package(JS_CMP_LEXER REQUIRED)

target_link_libraries(your_project PRIVATE JS_CMP_LEXER)
```

### Build Options

```bash
# Release build (optimized)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Debug build (with symbols)
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Custom install prefix
cmake -B build -DCMAKE_INSTALL_PREFIX=/usr/local
```

## 📁 Project Structure

```
Lexer/
├── include/                  # Public headers
│   ├── AST/                 # Abstract Syntax Tree
│   ├── Optimization/        # Optimization passes
│   └── Tokenizer/           # Core lexer components
├── src/                     # Source files
├── tests/                   # Unit tests
├── CMakeLists.txt           # Build configuration
└── README.md                 # This file
```

## 🤝 Contributing

We welcome contributions! Please follow these guidelines:

1. **Fork the repository** and create your feature branch
2. **Follow the code style** (see `.clang-format`)
3. **Write tests** for new functionality
4. **Update documentation** for changes
5. **Submit a pull request** with clear description

### Development Setup

```bash
# Install dependencies
sudo apt-get install cmake g++ libgtest-dev

# Build with tests
cmake -B build -DBUILD_TESTS=ON
cmake --build build
```

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

## 📬 Support

For questions, issues, or feature requests:

- **GitHub Issues**: https://github.com/JS-CMP/Lexer/issues
- **Discussions**: https://github.com/JS-CMP/Lexer/discussions
- **Email**: support@js-cmp.org

## 🚀 Roadmap

### Upcoming Features

- [ ] ECMAScript 6+ token support
- [ ] Enhanced error recovery
- [ ] Source map generation
- [ ] WebAssembly build target
- [ ] Python bindings

### Version History

- **v1.0**: Initial release with ES5 support
- **v1.1**: Performance optimizations
- **v1.2**: Enhanced error handling

## 📚 Additional Resources

- **ECMAScript 5 Specification**: https://www.ecma-international.org/ecma-262/5.1/
- **C++23 Documentation**: https://en.cppreference.com/w/cpp/23
- **CMake Documentation**: https://cmake.org/documentation/

---

© 2024 JS-CMP Project. All rights reserved.

*Built with ❤️ for JavaScript developers using C++*