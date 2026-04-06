/** ******************************************************************************
 * @file    TestHarness.cpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   Test harness implementation.
 *
 * @details
 * Implements the `TestHarness` methods to run tests, load tests from files,
 * report results to passed/failed files, and manage the test data directory.
 *
 *******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/

#include <cstdio>

#include "TestHarness.hpp"

/**
 * @Function TestHarness
 * @brief Constructor for TestHarness. Initializes test registry and ensures test directory exists.
 */
TestHarness::TestHarness() {
    test = new Tests();
    test->registerTests();
    ensureTestDirectoryExists();
}

/**
 * @Function ensureTestDirectoryExists
 * @brief Ensures the test data directory exists, creates it if not.
 * @return void
 */
void TestHarness::ensureTestDirectoryExists() {
    try {
        fs::create_directories(testDataDir);
    } catch (const fs::filesystem_error& e) {
        cerr << "Failed to create test directory: " << e.what() << endl;
        exit(1);
    }
}

/**
 * @Function removeLineFromFile
 * @brief Removes a specific line from a file.
 * @param filename Name of the file.
 * @param lineToRemove Line content to remove.
 * @return void
 */
void TestHarness::removeLineFromFile(const std::string& filename, const std::string& lineToRemove) {
    std::ifstream inFile(testDataDir + filename);
    if (!inFile) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    
    while (std::getline(inFile, line)) {
        if (line != lineToRemove) {
            lines.push_back(line);
        }
    }
    inFile.close();

    std::ofstream outFile(testDataDir + filename);
    if (!outFile) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    for (const auto& l : lines) {
        outFile << l << endl;
    }
    outFile.flush();
    outFile.close();
}

/**
 * @Function runTest
 * @brief Runs a single test by name.
 * @param testName Name of the test to run.
 * @return void
 */
void TestHarness::runTest(const std::string& testName) {
    auto funcConditonal = test->testRegistry.find(testName);
    if(funcConditonal != test->testRegistry.end()) {
        removeLineFromFile(passedfile, testName);
        removeLineFromFile(failedfile, testName);

        bool result = test->startTesting(
                    funcConditonal->second.keyEvents, 
                    funcConditonal->second.touchEvents, 
                    funcConditonal->second.func
                );
        cout << testName << ": " 
            << (result ? "PASSED" : "FAILED") << endl << endl;
        if (result) {
            std::ofstream passed(testDataDir + passedfile, std::ios::app);
            passed << testName << endl;
        } else {
            std::ofstream failed(testDataDir + failedfile, std::ios::app);
            failed << testName << endl;
        }
    } else {
        cerr << "Unknown test: " << testName << endl;
    }
}

/**
 * @Function runTestsFromFile
 * @brief Runs tests listed in the specified file.
 * @param targetTests Name of the file containing test names.
 * @return void
 */
void TestHarness::runTestsFromFile(
    const std::string& targetTests
) {
    cout << "Start Tests" << endl << endl;

    std::vector<std::string> targetContents;
    
    std::ifstream in(testDataDir + targetTests);
    if (!in.is_open()) {
        std::cerr << "Error: Could not open file " << testDataDir + targetTests << std::endl;
        return;
    }

    std::string line;
    while (std::getline(in, line)) {
        targetContents.push_back(line);
    }
    in.close();

    std::ofstream passed(testDataDir + passedfile, 
        (targetTests == failedfile) ? std::ios::app : std::ios::out);
    
    std::ofstream failed(testDataDir + failedfile, 
        (targetTests == passedfile) ? std::ios::app : std::ios::out);

    if (!passed) cerr << "Error opening " << passedfile << endl;
    if (!failed) cerr << "Error opening " << failedfile << endl;

    std::string testName;
    int total = 0, passedCount = 0, failedCount = 0;

    for (const auto& line : targetContents) {
        std::istringstream iss(line);
        if (iss >> testName) {
            total++;
            auto funcConditonal = test->testRegistry.find(testName);
            if(funcConditonal != test->testRegistry.end()) {
                bool result = test->startTesting(
                    funcConditonal->second.keyEvents, 
                    funcConditonal->second.touchEvents, 
                    funcConditonal->second.func
                );
                cout << testName << ": " 
                    << (result ? "PASSED" : "FAILED") << endl;
                if (result) {
                    passedCount++;
                    passed << testName << endl;
                } else {
                    failedCount++;
                    failed << testName << endl;
                }
            } else {
                cerr << "Unknown test: " << testName << endl;
            }
        }
    }
    
    cout << "\nSummary: " << passedCount << "/" << total << " tests passed"
        << " Failed: " << failedCount << " tests" << endl << endl;
    passed.flush();
    failed.flush();
}