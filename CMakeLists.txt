cmake_minimum_required(VERSION 3.10...3.31)
project(JS_CMP_LEXER VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_WARN_DEPRECATED OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Coverage support
option(ENABLE_COVERAGE "Enable code coverage analysis" OFF)

include_directories(include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "src/*.cpp"
        "include/*.hpp"
)



add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${INSTALL_CONFIGDIR}
)

# Coverage configuration
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Coverage analysis enabled")
        
        # Add required compiler and linker flags for coverage
        add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
        add_link_options(--coverage)
        
        # Apply coverage flags to the main library as well
        target_compile_options(${PROJECT_NAME} PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(${PROJECT_NAME} PRIVATE --coverage)
        
        # Find coverage tools
        find_program(GCOV_PATH gcov)
        find_program(LCOV_PATH lcov)
        find_program(GENHTML_PATH genhtml)
        find_program(GCOVR_PATH gcovr)
        
        if(NOT GCOV_PATH)
            message(WARNING "gcov not found - coverage disabled")
            set(ENABLE_COVERAGE OFF)
        else()
            message(STATUS "Found gcov: ${GCOV_PATH}")
            if(LCOV_PATH)
                message(STATUS "Found lcov: ${LCOV_PATH}")
            endif()
            if(GENHTML_PATH)
                message(STATUS "Found genhtml: ${GENHTML_PATH}")
            endif()
            if(GCOVR_PATH)
                message(STATUS "Found gcovr: ${GCOVR_PATH}")
            endif()
        endif()
    else()
        message(WARNING "Coverage only supported with GCC/Clang compilers")
        set(ENABLE_COVERAGE OFF)
    endif()
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT ${PROJECT_NAME}Targets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
        NAMESPACE ${PROJECT_NAME}::
)

# Only include tests if this project is the main project.
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB_RECURSE SOURCES_TEST CONFIGURE_DEPENDS
            "tests/*.cpp"
            "tests/crash_recovery_test.cpp"
    )
    
    # Add test coverage sources to test executable
    file(GLOB_RECURSE TEST_COVERAGE_SOURCES CONFIGURE_DEPENDS
            "tests/TestCoverage/*.cpp"
    )
    
    list(APPEND SOURCES_TEST ${TEST_COVERAGE_SOURCES})

    add_executable(${PROJECT_NAME}_test ${SOURCES_TEST})

    target_link_libraries(${PROJECT_NAME}_test
            PRIVATE
            gtest
            gtest_main
            ${PROJECT_NAME}
    )
    
    target_include_directories(${PROJECT_NAME}_test
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME}_test)
    
    # Coverage targets
    if(ENABLE_COVERAGE)
        # Clean coverage data before tests
        add_custom_target(coverage_clean
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/coverage"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/coverage"
            COMMAND ${LCOV_PATH} --directory . --zerocounters
            COMMENT "Cleaning coverage data"
        )
        
        # Generate coverage report
        add_custom_target(coverage_report
            COMMAND ${LCOV_PATH} --directory . --capture --output-file "${CMAKE_BINARY_DIR}/coverage/coverage.info"
            COMMAND ${LCOV_PATH} --remove "${CMAKE_BINARY_DIR}/coverage/coverage.info" "tests/*" "/usr/*" --output-file "${CMAKE_BINARY_DIR}/coverage/coverage_filtered.info"
            COMMAND ${GENHTML_PATH} "${CMAKE_BINARY_DIR}/coverage/coverage_filtered.info" --output-directory "${CMAKE_BINARY_DIR}/coverage/html"
            COMMAND ${GCOVR_PATH} --xml "${CMAKE_BINARY_DIR}/coverage/coverage_filtered.info" --output "${CMAKE_BINARY_DIR}/coverage/coverage.xml"
            COMMAND ${GCOVR_PATH} --html-details "${CMAKE_BINARY_DIR}/coverage/coverage_filtered.info" --output "${CMAKE_BINARY_DIR}/coverage/gcovr_html/index.html"
            COMMENT "Generating coverage reports"
        )
        
        # Combined target: clean, test, report
        add_custom_target(coverage
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target coverage_clean
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME}_test
            COMMAND ctest --output-on-failure
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target coverage_report
            COMMENT "Running tests with coverage analysis"
        )
    endif()
endif()
