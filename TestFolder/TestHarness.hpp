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

/*
File names of more used files along with testDataDir which is the directory of the test text files
*/
inline const std::string alltestsfile = "Tests.txt";
inline const std::string passedfile = "passedTests.txt";
inline const std::string failedfile = "failedTests.txt";
inline const std::string testDataDir = "TestFolder/TestData/";

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

/**
 * @brief Class that has a test harness for running the tests and keeping track of passed and failed tests 
 * and allowing the passed and failed tests to be re run as they take from a file
 * 
 */
class TestHarness {
public:

    TestHarness();

    /**
     * @brief Runs a single test by name as a string
     * 
     * Outputs to passedTests and failedTests in TestData folder
     * 
     * Test must exist in testRegistry
     * 
     * @param testName name of test as a string
     */
    void runTest(const std::string& testName);

    /**
     * @brief Runs tests from a file seperated by new lines
     * 
     * Outputs to passedTests and failedTests in TestData folder
     * 
     * Tests must exist in testRegistry
     * 
     * @param targetTests string name of tests file in TestData folder
     */
    void runTestsFromFile(
        const std::string& targetTests
    );

private:

    Tests* test;

    bool loopCondition = true;

    /**
     * @brief Checks the TestFolder exists
     * 
     */
    void ensureTestDirectoryExists();

    /**
     * @brief Used to remove a test
     * 
     * @param filename testfile
     * @param lineToRemove testname
     */
    void removeLineFromFile(
        const std::string& filename, 
        const std::string& lineToRemove
    );


};