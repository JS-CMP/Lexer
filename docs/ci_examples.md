# CI Integration Examples

This document provides examples of how to integrate the JS-CMP Lexer test coverage reporting into various CI systems.

## GitHub Actions

See `.github/workflows/coverage.yml` for a complete GitHub Actions workflow.

### Basic Setup

```yaml
name: JS-CMP Lexer CI

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v4
      with:
        submodules: recursive
    
    - name: Install dependencies
      run: sudo apt-get install -y lcov gcovr
    
    - name: Build with coverage
      run: |
        cmake -DENABLE_COVERAGE=ON -B build
        cmake --build build
    
    - name: Run tests
      run: ./build/JS_CMP_LEXER_test
    
    - name: Upload reports
      uses: actions/upload-artifact@v3
      with:
        name: test-reports
        path: build/reports/
```

## GitLab CI

```yaml
stages:
  - test
  - report

variables:
  BUILD_DIR: build

test_job:
  stage: test
  image: ubuntu:latest
  
  before_script:
    - apt-get update && apt-get install -y cmake g++ lcov gcovr
  
  script:
    - cmake -DENABLE_COVERAGE=ON -B $BUILD_DIR
    - cmake --build $BUILD_DIR
    - ./$BUILD_DIR/JS_CMP_LEXER_test
  
  artifacts:
    paths:
      - $BUILD_DIR/reports/
    expire_in: 1 week

coverage_report:
  stage: report
  image: ubuntu:latest
  
  script:
    - echo "Coverage reports generated"
    - ls -la $BUILD_DIR/reports/
  
  dependencies:
    - test_job
```

## Jenkins

```groovy
pipeline {
    agent any
    
    stages {
        stage('Checkout') {
            steps {
                checkout([
                    $class: 'GitSCM',
                    branches: [[name: '*/main']],
                    extensions: [[$class: 'SubmoduleOption', recursive: true]],
                    userRemoteConfigs: [[url: 'https://github.com/your-repo/JS-CMP-Lexer.git']]
                ])
            }
        }
        
        stage('Install Dependencies') {
            steps {
                sh 'sudo apt-get update && sudo apt-get install -y cmake g++ lcov gcovr'
            }
        }
        
        stage('Build with Coverage') {
            steps {
                sh 'cmake -DENABLE_COVERAGE=ON -B build'
                sh 'cmake --build build'
            }
        }
        
        stage('Run Tests') {
            steps {
                sh './build/JS_CMP_LEXER_test'
            }
        }
        
        stage('Archive Reports') {
            steps {
                archiveArtifacts artifacts: 'build/reports/**', fingerprint: true
            }
        }
    }
    
    post {
        always {
            junit '**/test-results.xml' // If you add JUnit output
            publishHTML(target: [
                allowMissing: false,
                alwaysLinkToLastBuild: true,
                keepAll: true,
                reportDir: 'build/reports',
                reportFiles: 'coverage_report.html',
                reportName: 'Coverage Report'
            ])
        }
    }
}
```

## CircleCI

```yaml
version: 2.1

jobs:
  build_and_test:
    docker:
      - image: ubuntu:latest
    
    steps:
      - checkout
      - run:
          name: Install dependencies
          command: apt-get update && apt-get install -y cmake g++ lcov gcovr
      - run:
          name: Configure CMake
          command: cmake -DENABLE_COVERAGE=ON -B build
      - run:
          name: Build
          command: cmake --build build
      - run:
          name: Run tests
          command: ./build/JS_CMP_LEXER_test
      - store_artifacts:
          path: build/reports
          destination: test-reports

workflows:
  version: 2
  test_workflow:
    jobs:
      - build_and_test
```

## Azure Pipelines

```yaml
trigger:
- main

pool:
  vmImage: 'ubuntu-latest'

steps:
- checkout: self
  submodules: recursive

- script: sudo apt-get update && sudo apt-get install -y lcov gcovr
  displayName: 'Install dependencies'

- script: cmake -DENABLE_COVERAGE=ON -B build
  displayName: 'Configure CMake'

- script: cmake --build build
  displayName: 'Build'

- script: ./build/JS_CMP_LEXER_test
  displayName: 'Run tests'

- task: PublishBuildArtifacts@1
  inputs:
    PathtoPublish: 'build/reports'
    ArtifactName: 'test-reports'
    publishLocation: 'Container'

- task: PublishTestResults@2
  condition: succeededOrFailed()
  inputs:
    testResultsFiles: '**/test-results.xml'
    testRunTitle: 'JS-CMP Lexer Tests'
```

## Travis CI

```yaml
language: cpp

os: linux

dist: focal

addons:
  apt:
    packages:
      - cmake
      - g++
      - lcov
      - python3-pip

before_install:
  - pip3 install gcovr

script:
  - cmake -DENABLE_COVERAGE=ON -B build
  - cmake --build build
  - ./build/JS_CMP_LEXER_test

after_success:
  - mkdir -p build/reports
  - echo "Test coverage reports generated"

cache:
  directories:
    - $HOME/.cache/pip
```

## CI Best Practices

### Environment Setup

1. **Install Dependencies**: Ensure all required tools are installed
2. **Submodules**: Use `submodules: recursive` for GitHub submodules
3. **Cache**: Cache dependencies to speed up builds

### Test Execution

1. **Build Matrix**: Test on multiple platforms (Linux, macOS, Windows)
2. **Parallel Jobs**: Run tests in parallel where possible
3. **Artifacts**: Always archive test reports for debugging

### Coverage Analysis

1. **Tool Availability**: Check if coverage tools are available
2. **Fallback**: Provide fallback when tools are missing
3. **Thresholds**: Set minimum coverage thresholds

### Reporting

1. **Multiple Formats**: Generate HTML, XML, and console reports
2. **Artifacts**: Upload all report formats
3. **Notifications**: Send notifications on test failures

## Troubleshooting CI Issues

### Common Problems

1. **Missing Tools**: Ensure lcov/gcovr are installed
2. **Path Issues**: Use absolute paths for artifacts
3. **Permissions**: Ensure write permissions for report directories
4. **Submodules**: Verify submodules are checked out correctly

### Debugging Tips

1. **Verbose Output**: Add `-v` flags to build commands
2. **Step Isolation**: Run steps individually to isolate issues
3. **Environment Info**: Print environment variables for debugging
4. **Tool Versions**: Verify tool versions match expectations

## CI Configuration Checklist

- [ ] Install required dependencies (cmake, g++, lcov, gcovr)
- [ ] Configure CMake with coverage enabled
- [ ] Build project successfully
- [ ] Run tests and generate reports
- [ ] Archive test reports as artifacts
- [ ] Set up notifications for test failures
- [ ] Configure minimum coverage thresholds (optional)
- [ ] Set up test result publishing (optional)