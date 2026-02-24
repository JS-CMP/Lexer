#!/bin/bash

# Coverage Analysis Script for JS_CMP_LEXER
# This script automates the build-test-coverage workflow
# 
# Usage: ./scripts/coverage.sh [options]
# 
# Options:
#   -h, --help      Show this help message
#   -c, --clean     Clean build and coverage data before running
#   -n, --no-open   Don't automatically open HTML report
#   -v, --verbose   Show verbose output
# 
# Features:
#   - Automatic tool detection and error handling
#   - Builds project with coverage instrumentation
#   - Runs all tests to collect coverage data
#   - Generates multiple report formats (HTML, XML, text)
#   - Filters out test code and system headers
#   - Automatic HTML report opening
#   - Graceful fallback when some tools are missing
# 
# Requirements:
#   - gcov (critical - must be installed)
#   - lcov (recommended - for best results)
#   - gcovr (recommended - for additional reports)
#   - genhtml (recommended - for HTML reports)
# 
# Installation on macOS:
#   brew install lcov gcovr
# 
# Installation on Ubuntu/Debian:
#   sudo apt-get install lcov gcovr

set -e  # Exit on error

# Parse command line arguments
CLEAN_BUILD=false
AUTO_OPEN=true
VERBOSE=false

while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help)
            echo "Usage: $0 [options]"
            echo ""
            echo "Options:"
            echo "  -h, --help      Show this help message"
            echo "  -c, --clean     Clean build and coverage data before running"
            echo "  -n, --no-open   Don't automatically open HTML report"
            echo "  -v, --verbose   Show verbose output"
            exit 0
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -n|--no-open)
            AUTO_OPEN=false
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            set -x  # Enable verbose mode
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

# Configuration
BUILD_DIR="build"
COVERAGE_DIR="$BUILD_DIR/coverage"
REPORT_DIR="$COVERAGE_DIR/reports"
PROJECT_NAME="JS_CMP_LEXER"
TEST_EXECUTABLE="$BUILD_DIR/$PROJECT_NAME_test"

# Tool availability flags
HAVE_GCOVR=false
HAVE_LCOV=false
HAVE_GENHTML=false

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'  # No Color

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to display error messages
error_exit() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
    exit 1
}

# Function to display info messages
info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

# Function to display success messages
success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

# Function to display warning messages
warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Check for required tools
check_tools() {
    info "Checking for required coverage tools..."
    
    missing_critical_tools=()
    
    # Check for gcov (critical)
    if ! command_exists gcov; then
        missing_critical_tools+=("gcov")
    fi
    
    # Check for lcov
    if command_exists lcov; then
        HAVE_LCOV=true
        success "Found lcov"
    else
        warning "lcov not found - some report features will be disabled"
    fi
    
    # Check for gcovr
    if command_exists gcovr; then
        HAVE_GCOVR=true
        success "Found gcovr"
    else
        warning "gcovr not found - some report features will be disabled"
    fi
    
    # Check for genhtml
    if command_exists genhtml; then
        HAVE_GENHTML=true
        success "Found genhtml"
    else
        warning "genhtml not found - some report features will be disabled"
    fi
    
    # If critical tools are missing, provide installation instructions
    if [ ${#missing_critical_tools[@]} -gt 0 ]; then
        error_exit "The following critical tools are missing: ${missing_critical_tools[*]}"
        echo ""
        echo "Installation instructions:"
        echo "  On macOS (using Homebrew):"
        echo "    brew install lcov gcovr"
        echo ""
        echo "  On Ubuntu/Debian:"
        echo "    sudo apt-get install lcov gcovr"
        echo ""
        echo "  On other systems:"
        echo "    Please install the missing tools using your package manager"
    fi
    
    success "Tool check completed"
}

# Clean previous coverage data
clean_coverage() {
    info "Cleaning previous coverage data..."
    
    # If --clean was specified, also clean the build directory
    if [ "$CLEAN_BUILD" = true ]; then
        info "Clean build requested - removing build directory..."
        if [ -d "$BUILD_DIR" ]; then
            rm -rf "$BUILD_DIR"
        fi
    fi
    
    # Remove coverage directory if it exists
    if [ -d "$COVERAGE_DIR" ]; then
        rm -rf "$COVERAGE_DIR"
    fi
    
    # Create fresh directories
    mkdir -p "$COVERAGE_DIR"
    mkdir -p "$REPORT_DIR"
    
    # Reset coverage counters
    if [ "$HAVE_LCOV" = true ]; then
        lcov --directory . --zerocounters
    fi
    
    success "Coverage data cleaned"
}

# Configure CMake with coverage
configure_cmake() {
    info "Configuring CMake with coverage support..."
    
    # Check if build directory exists, if not create it
    if [ ! -d "$BUILD_DIR" ]; then
        mkdir -p "$BUILD_DIR"
    fi
    
    # Configure with coverage enabled
    cmake -DENABLE_COVERAGE=ON -B "$BUILD_DIR"
    
    success "CMake configured with coverage support"
}

# Build the project
build_project() {
    info "Building project..."
    
    cmake --build "$BUILD_DIR" --config Debug
    
    success "Project built successfully"
}

# Run tests
run_tests() {
    info "Running tests..."
    
    # Check if test executable exists
    if [ ! -f "$TEST_EXECUTABLE" ]; then
        # Try to find the test executable with different patterns
        if [ -f "$BUILD_DIR/JS_CMP_LEXER_test" ]; then
            TEST_EXECUTABLE="$BUILD_DIR/JS_CMP_LEXER_test"
        elif [ -f "$BUILD_DIR/${PROJECT_NAME}_test" ]; then
            TEST_EXECUTABLE="$BUILD_DIR/${PROJECT_NAME}_test"
        else
            # Try to find any executable that looks like a test
            potential_tests=$(find "$BUILD_DIR" -name "*test*" -type f -executable | head -5)
            if [ -n "$potential_tests" ]; then
                echo "Potential test executables found:"
                echo "$potential_tests"
            fi
            
            # List what's in the build directory for debugging
            echo "Executables in build directory:"
            find "$BUILD_DIR" -type f -executable | grep -v ".dylib" | grep -v ".so" | head -10
            error_exit "Test executable not found: $TEST_EXECUTABLE"
        fi
    fi
    
    # Run tests
    ctest --test-dir "$BUILD_DIR" --output-on-failure || true
    
    success "Tests completed"
}

# Generate coverage reports
generate_coverage() {
    info "Generating coverage reports..."
    
    # Create coverage directory if it doesn't exist
    mkdir -p "$COVERAGE_DIR"
    mkdir -p "$REPORT_DIR"
    
    # Use lcov if available for basic coverage capture
    if [ "$HAVE_LCOV" = true ]; then
        info "Using lcov for coverage data capture..."
        
        # Capture coverage data (ignore common lcov errors)
        lcov --directory . --capture --output-file "$COVERAGE_DIR/coverage.info" --ignore-errors unsupported,inconsistent
        
        # Extract only our source files (better approach than removing)
        lcov --extract "$COVERAGE_DIR/coverage.info" "*Lexer*" --output-file "$COVERAGE_DIR/coverage_filtered.info" --ignore-errors unsupported,inconsistent,empty,unused
        
        # Generate HTML report with genhtml if available
        if [ "$HAVE_GENHTML" = true ]; then
            info "Generating HTML report with genhtml..."
            # Create a temporary file with more aggressive error ignoring
            genhtml "$COVERAGE_DIR/coverage_filtered.info" --output-directory "$REPORT_DIR/html" --ignore-errors unsupported,inconsistent,corrupt,empty,unused 2>/dev/null || {
                warning "genhtml failed with standard approach, trying alternative..."
                # Try a more direct approach with maximum error ignoring
                genhtml --ignore-errors all "$COVERAGE_DIR/coverage_filtered.info" --output-directory "$REPORT_DIR/html" 2>/dev/null || {
                    warning "genhtml failed - HTML report generation skipped"
                    echo "HTML report generation failed due to coverage data inconsistencies" > "$REPORT_DIR/html/index.html"
                    echo "This is often caused by compiler optimizations or template code." >> "$REPORT_DIR/html/index.html"
                    echo "The text summary and raw coverage data are still available." >> "$REPORT_DIR/html/index.html"
                }
            }
        else
            warning "genhtml not available - HTML report will not be generated"
        fi
    else
        warning "lcov not available - using basic gcov data collection"
        # Fallback: just ensure we have the coverage directory
        touch "$COVERAGE_DIR/coverage_basic.info"
    fi
    
    # Use gcovr if available for additional reports
    if [ "$HAVE_GCOVR" = true ]; then
        info "Using gcovr for additional coverage reports..."
        
        if [ "$HAVE_LCOV" = true ]; then
            # Use the filtered lcov data if available
            gcovr --xml "$COVERAGE_DIR/coverage_filtered.info" --output "$REPORT_DIR/coverage.xml"
            gcovr --html-details "$COVERAGE_DIR/coverage_filtered.info" --output "$REPORT_DIR/gcovr_html/index.html"
            gcovr "$COVERAGE_DIR/coverage_filtered.info" --txt --output "$REPORT_DIR/coverage_summary.txt"
        else
            # Fallback to direct coverage collection
            gcovr --xml --output "$REPORT_DIR/coverage.xml"
            gcovr --html-details --output "$REPORT_DIR/gcovr_html/index.html"
            gcovr --txt --output "$REPORT_DIR/coverage_summary.txt"
        fi
    else
        warning "gcovr not available - XML and detailed reports will not be generated"
        
        # Try to generate a basic text summary using available tools
        if [ "$HAVE_LCOV" = true ]; then
            lcov --summary "$COVERAGE_DIR/coverage_filtered.info" --ignore-errors unsupported,inconsistent > "$REPORT_DIR/coverage_summary.txt" || {
                warning "Failed to generate text summary"
                echo "Coverage data available but summary generation failed" > "$REPORT_DIR/coverage_summary.txt"
                echo "Try: lcov --summary $COVERAGE_DIR/coverage_filtered.info --ignore-errors unsupported,inconsistent" >> "$REPORT_DIR/coverage_summary.txt"
            }
        fi
    fi
    
    success "Coverage reports generated in $REPORT_DIR"
}

# Display coverage summary
display_summary() {
    info "Displaying coverage summary..."
    
    echo -e "\n${BLUE}=== COVERAGE SUMMARY ===${NC}"
    
    # Display the text summary
    if [ -f "$REPORT_DIR/coverage_summary.txt" ]; then
        cat "$REPORT_DIR/coverage_summary.txt"
    else
        warning "Coverage summary not found - some tools may not be available"
        echo "Available reports:"
        ls -la "$REPORT_DIR/" 2>/dev/null || echo "No reports found"
    fi
    
    echo -e "\n${BLUE}=== REPORT LOCATIONS ===${NC}"
    
    # Only show available reports
    if [ -d "$REPORT_DIR/html" ] && [ -f "$REPORT_DIR/html/index.html" ]; then
        echo "HTML Report (genhtml):   $REPORT_DIR/html/index.html"
    else
        echo "HTML Report (genhtml):   ${YELLOW}Not available${NC}"
    fi
    
    if [ -d "$REPORT_DIR/gcovr_html" ] && [ -f "$REPORT_DIR/gcovr_html/index.html" ]; then
        echo "HTML Report (gcovr):     $REPORT_DIR/gcovr_html/index.html"
    else
        echo "HTML Report (gcovr):     ${YELLOW}Not available${NC}"
    fi
    
    if [ -f "$REPORT_DIR/coverage.xml" ]; then
        echo "XML Report:              $REPORT_DIR/coverage.xml"
    else
        echo "XML Report:              ${YELLOW}Not available${NC}"
    fi
    
    if [ -f "$REPORT_DIR/coverage_summary.txt" ]; then
        echo "Text Summary:            $REPORT_DIR/coverage_summary.txt"
    else
        echo "Text Summary:            ${YELLOW}Not available${NC}"
    fi
    
    if [ -f "$COVERAGE_DIR/coverage_filtered.info" ]; then
        echo "Raw Coverage Data:       $COVERAGE_DIR/coverage_filtered.info"
    else
        echo "Raw Coverage Data:       ${YELLOW}Not available${NC}"
    fi
}

# Open HTML report automatically
open_report() {
    # If auto-open is disabled, skip this function
    if [ "$AUTO_OPEN" = false ]; then
        info "Auto-open disabled - skipping HTML report opening"
        return
    fi
    
    info "Attempting to open HTML coverage report..."
    
    # Try to find an available HTML report
    html_report=""
    
    # Prefer genhtml report if available
    if [ -f "$REPORT_DIR/html/index.html" ]; then
        html_report="$REPORT_DIR/html/index.html"
    # Fallback to gcovr HTML report
    elif [ -f "$REPORT_DIR/gcovr_html/index.html" ]; then
        html_report="$REPORT_DIR/gcovr_html/index.html"
    fi
    
    # If we found an HTML report, try to open it
    if [ -n "$html_report" ]; then
        # Try different browsers/commands to open the report
        if command_exists xdg-open; then
            xdg-open "$html_report" &
        elif command_exists open; then
            open "$html_report" &
        elif command_exists google-chrome; then
            google-chrome "$html_report" &
        elif command_exists firefox; then
            firefox "$html_report" &
        else
            warning "Could not automatically open HTML report. Please open it manually:"
            echo "File: $html_report"
        fi
    else
        warning "No HTML report available to open"
        # Create a simple HTML report with the text summary
        if [ -f "$REPORT_DIR/coverage_summary.txt" ]; then
            info "Creating simple HTML report from text summary..."
            echo "<html><head><title>Coverage Report</title></head><body>" > "$REPORT_DIR/html/index.html"
            echo "<h1>Coverage Report</h1>" >> "$REPORT_DIR/html/index.html"
            echo "<p>HTML report generation encountered issues. Here's the text summary:</p>" >> "$REPORT_DIR/html/index.html"
            echo "<pre>" >> "$REPORT_DIR/html/index.html"
            cat "$REPORT_DIR/coverage_summary.txt" >> "$REPORT_DIR/html/index.html"
            echo "</pre>" >> "$REPORT_DIR/html/index.html"
            echo "</body></html>" >> "$REPORT_DIR/html/index.html"
            html_report="$REPORT_DIR/html/index.html"
            # Try to open the fallback HTML report
            if command_exists xdg-open; then
                xdg-open "$html_report" &
            elif command_exists open; then
                open "$html_report" &
            fi
        fi
    fi
}

# Main function
main() {
    echo -e "${BLUE}=== JS_CMP_LEXER COVERAGE ANALYSIS ===${NC}"
    echo "This script will build, test, and generate coverage reports"
    echo ""
    
    # Check for required tools
    check_tools
    
    # Clean previous coverage data
    clean_coverage
    
    # Configure CMake
    configure_cmake
    
    # Build project
    build_project
    
    # Run tests
    run_tests
    
    # Generate coverage reports
    generate_coverage
    
    # Display summary
    display_summary
    
    # Open report
    open_report
    
    echo ""
    success "Coverage analysis completed successfully!"
    echo -e "${YELLOW}Reports are available in: $REPORT_DIR${NC}"
}

# Run main function
main "$@"