/** ******************************************************************************
 * @file    TestHarness.hpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   Test harness declarations and utilities.
 *
 * @details
 * Declares the `TestHarness` class used to run individual or batched tests,
 * manage test result files, and coordinate with the `Tests` registry.
 * Includes helpers for file handling and ensuring test directories exist.
 *
 *******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
# *******************************************************************************/

#pragma once
#include <cstdio>

#include <map>
#include <functional>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

/**
File names of more used files along with testDataDir which is the directory of the test text files
*/
inline const std::string alltestsfile = "Tests.txt";
inline const std::string passedfile = "passedTests.txt";
inline const std::string failedfile = "failedTests.txt";
inline const std::string testDataDir = "TestFolder/TestData/";
inline const std::string ManualTests = "ManualTests.txt";

inline const std::string displayTests = "DisplayTests.txt";
inline const std::string drawTests = "DrawTests.txt";
inline const std::string imageTests = "ImageTests.txt";
inline const std::string keyTests = "KeyTests.txt";
inline const std::string statusTests = "StatusTests.txt";
inline const std::string stringTests = "StringTests.txt";
inline const std::string otherTests = "OtherTests.txt";

namespace fs = std::filesystem;
using std::endl;
using std::cerr;
using std::cout;

#include "Tests.hpp"


class TestHarness {
public:
    /**
     * @Function TestHarness
     * @brief Constructor for TestHarness. Initializes test registry and ensures test directory exists.
     */
    TestHarness();

    /**
     * @Function runTest
     * @brief Runs a single test by name.
     * @param testName Name of the test to run.
     * @return void
     */
    void runTest(const std::string& testName);

    /**
     * @Function runTestsFromFile
     * @brief Runs tests listed in the specified file.
     * @param targetTests Name of the file containing test names.
     * @return void
     */
    void runTestsFromFile(
        const std::string& targetTests
    );

private:
    Tests* test;
    bool loopCondition = true;

    /**
     * @Function ensureTestDirectoryExists
     * @brief Ensures the test data directory exists, creates it if not.
     * @return void
     */
    void ensureTestDirectoryExists();

    /**
     * @Function removeLineFromFile
     * @brief Removes a specific line from a file.
     * @param filename Name of the file.
     * @param lineToRemove Line content to remove.
     * @return void
     */
    void removeLineFromFile(
        const std::string& filename, 
        const std::string& lineToRemove
    );
};