#include <cstdio>

#include "TestHarness.hpp"

/**
 * @brief Program using the Test Harness
 * 
 * @return int 
 */

int main()
{
    //Instantiate the test harness object
    TestHarness* testHarness = new TestHarness();
 
    //Run all tests using "runTestsFromFile" followed by the file name
    /**
     * alltestsfile is a constant containing the all tests file name
     * So are failedfile, passedfile and imageTests
     */
    //testHarness->runTestsFromFile(alltestsfile);

    //Rerun failed
    //testHarness->runTestsFromFile(failedfile);

    //Rerun passed
    //testHarness->runTestsFromFile(passedfile);

    //Single test
    //testHarness->runTest("update_Single_Column_Functionality");

    //Group of tests using a file
    testHarness->runTestsFromFile(statusTests);

    return 0;
}