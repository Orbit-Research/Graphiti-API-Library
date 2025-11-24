TestFolder
- Files in TestData folder are txt files used by the Test Harness
- passedTests.txt and failedTests.txt hold the passed and failed test names
- Any .txt file in TestData can be run using:
    testHarness->runTestsFromFile(passedfile);
- Running a single test can be done like so:
    testHarness->runTest("clear_Display_Functionality")
- There are shorthand constant strings in the top of TestHarness.cpp for runing test files easily

- To exit the Key test press the select button
- To exit the Draw test draw into the bottom right corner of the Graphiti Unit
- The device must be in draw mode for the draw test to execute properly

- Running any tests will write the failed and passed tests to their .txt files

- Note: The tools.ps1 scripts use Ninja. Please use the equaivalent commands in scripts/tools.ps1 to build your code if you are not using Ninja.

- The tests can be run by running:

    .\scripts\tools.ps1 standard  

        OR

    .\scripts\tools.ps1 standard_debug

## Notes ##

- Some test may need to be updated to your exact software version hardware version etc. due to the test matching exactly against the specified string output
    - This can be done in Tests.cpp
- Test can be set to VCP or HID by changing the startTesting function in Tests.cpp to use startUpHID or startUpVCP and shutDownVCP or shutDownHID which are commented out. One must also change the COM port for VCP and alternatively change the HID numbers if they have changed

## File Structure ##
- TestData
    - Folder containing txt files that have the test names for the test harness
    - Files with specific test names can be used for running that set of tests
    - passedTests.txt will always contain the passed tests from the most recent run and previous runs
    - failedTests.txt will always contain the failed tests from the most recent run and previous runs
    - Tests.txt includes all tests

- main.cpp
    - Shows use of the test harness 

- TestHarness.hpp
    - Test Harness header file

- TestHarness.cpp
    - Test Harness function implementation

- Tests.hpp
    - Tests header file

- Tests.cpp
        - Implementation of the tests