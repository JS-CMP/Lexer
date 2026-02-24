#include <gtest/gtest.h>
#include "TestUtils.hpp"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class BasicTestSuite : public ::testing::Test {
protected:
    void SetUp() override {
        // Find all test files in the basic test directory
        testFiles = TestUtils::findTestFiles("tests/cases/basic");
    }
    
    std::vector<std::string> testFiles;
};

// Test fixture for individual file tests
class JavaScriptFileTest : public ::testing::TestWithParam<std::string> {
protected:
    void SetUp() override {
        jsFilePath = GetParam();
        
        // Generate expected output file path by replacing .js with .out
        outFilePath = jsFilePath;
        size_t dotPos = outFilePath.find_last_of('.');
        if (dotPos != std::string::npos) {
            outFilePath = outFilePath.substr(0, dotPos) + ".out";
        }
    }
    
    std::string jsFilePath;
    std::string outFilePath;
};

// Test that processes a single JavaScript file and compares with expected output
TEST_P(JavaScriptFileTest, ProcessAndCompare) {
    try {
        // Process the JavaScript file
        std::string actualOutput = TestUtils::processJavaScriptFile(jsFilePath);
        
        // Read expected output
        std::string expectedOutput = TestUtils::readExpectedOutput(outFilePath);
        
        // Compare outputs (with whitespace normalization)
        bool match = TestUtils::compareOutputs(actualOutput, expectedOutput);
        
        if (!match) {
            std::cerr << "Output mismatch for: " << jsFilePath << std::endl;
            std::cerr << "Expected output file: " << outFilePath << std::endl;
            
            // For debugging, you might want to write the actual output to a file
            // std::ofstream debugFile(outFilePath + ".debug");
            // debugFile << actualOutput;
            // debugFile.close();
        }
        
        ASSERT_TRUE(match) << "Transpiled output does not match expected output for: " << jsFilePath;
        
    } catch (const std::exception& e) {
        FAIL() << "Error processing " << jsFilePath << ": " << e.what();
    }
}

// Test that verifies all test files have corresponding .out files
TEST_F(BasicTestSuite, AllFilesHaveExpectedOutput) {
    for (const auto& jsFile : testFiles) {
        std::string expectedOutFile = jsFile;
        size_t dotPos = expectedOutFile.find_last_of('.');
        if (dotPos != std::string::npos) {
            expectedOutFile = expectedOutFile.substr(0, dotPos) + ".out";
        }
        
        ASSERT_TRUE(fs::exists(expectedOutFile)) 
            << "Missing expected output file for " << jsFile 
            << ". Expected: " << expectedOutFile;
    }
}

// Test that verifies we can find test files
TEST_F(BasicTestSuite, CanFindTestFiles) {
    ASSERT_FALSE(testFiles.empty()) << "No test files found in tests/cases/basic";
    
    for (const auto& file : testFiles) {
        ASSERT_TRUE(fs::exists(file)) << "Test file does not exist: " << file;
    }
}

// Function to get all test files
std::vector<std::string> getAllTestFiles() {
    return TestUtils::findTestFiles("tests/cases/basic");
}

// Instantiate the parameterized test with all JavaScript files
INSTANTIATE_TEST_SUITE_P(
    BasicJavaScriptTests,
    JavaScriptFileTest,
    ::testing::ValuesIn(getAllTestFiles())
);

// Main function to run all tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}