#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "../include/Lexer.hpp"
#include "../include/Program.hpp"

namespace fs = std::filesystem;

std::string readFile(const std::string& filePath) {
    std::ifstream fileStream(filePath);
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

void writeFile(const std::string& filePath, const Lexer::Program& content) {
    std::ofstream fileStream(filePath);
    fileStream << content;
}

TEST(AutoGeneratedTests /*unused*/, AllCases /*unused*/) {
    std::string baseDir = "tests/cases";

    for (const auto& entry : fs::recursive_directory_iterator(baseDir)) {
        if (entry.path().extension() == ".js") {
            std::string js_path = entry.path().string();
            std::filesystem::path cpp_path = entry.path();
            cpp_path.replace_extension(".out");
            std::filesystem::path out_path = entry.path();
            out_path.replace_extension(".tmp");

            if (fs::exists(cpp_path)) {
                std::string input = readFile(js_path);
                std::string expected_output = readFile(cpp_path);

                Lexer::Lexer lexer(input);
                std::vector<Lexer::Token> tokens = lexer.tokenize();
                Lexer::Program program(tokens);
                program.parse();


                std::stringstream program_output;
                program_output << program;
                writeFile(out_path, program);
                EXPECT_EQ(program_output.str(), expected_output)
                            << "Output mismatch for " << js_path << ". Check " << cpp_path;
            } else {
                FAIL() << "Missing expected output file for " << js_path << ": " << cpp_path;
            }
        }
    }
}
