cmake_minimum_required(VERSION 3.25)
project(JS_CMP_Lexer_Examples)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the lexer package
find_package(JS_CMP_LEXER REQUIRED)

# Add examples
add_executable(basic_tokenization basic_tokenization.cpp)
target_link_libraries(basic_tokenization PRIVATE JS_CMP_LEXER)

add_executable(streaming_tokenization streaming_tokenization.cpp)
target_link_libraries(streaming_tokenization PRIVATE JS_CMP_LEXER)

add_executable(token_analysis token_analysis.cpp)
target_link_libraries(token_analysis PRIVATE JS_CMP_LEXER)

# Include directories
target_include_directories(basic_tokenization PRIVATE ${JS_CMP_LEXER_INCLUDE_DIRS})
target_include_directories(streaming_tokenization PRIVATE ${JS_CMP_LEXER_INCLUDE_DIRS})
target_include_directories(token_analysis PRIVATE ${JS_CMP_LEXER_INCLUDE_DIRS})