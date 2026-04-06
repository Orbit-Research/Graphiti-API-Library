/** ******************************************************************************
 * @file    main.cpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   Test harness runner.
 *
 * @details
 * Small CLI program that creates a `TestHarness` instance and runs test
 * suites defined in `TestFolder/TestData`. Intended for developers to run
 * automated or manual test groups against a connected Graphiti device.
 *
 *****************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 ***************************************************************************** */

#include <iostream>
#include <limits>
#include "TestHarness.hpp"

/**
 * @function main
 * @brief Interactive CLI test suite runner for Graphiti API
 * @details Presents a menu-driven interface allowing users to select and run
 *          different test suites (displayTests, drawTests, imageTests, keyTests,
 *          statusTests, stringTests, otherTests) against a connected Graphiti device.
 *          Continues running until user selects Exit option.
 * @param argc : int - Number of command-line arguments (unused)
 * @param argv : char** - Command-line argument array (unused)
 * @return int - Exit status: 0 on successful completion, non-zero on error
 */
int main()
{
    TestHarness* testHarness = new TestHarness();

    int choice = 0;

    while (true)
    {
        std::cout << "\nSelect Test Suite\n";
        std::cout << "1. displayTests\n";
        std::cout << "2. drawTests\n";
        std::cout << "3. imageTests\n";
        std::cout << "4. keyTests\n";
        std::cout << "5. statusTests\n";
        std::cout << "6. stringTests\n";
        std::cout << "7. otherTests\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter choice: ";

        std::cin >> choice;

        // Handle invalid input (non-integer)
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                testHarness->runTestsFromFile(displayTests);
                break;
            case 2:
                testHarness->runTestsFromFile(drawTests);
                break;
            case 3:
                testHarness->runTestsFromFile(imageTests);
                break;
            case 4:
                testHarness->runTestsFromFile(keyTests);
                break;
            case 5:
                testHarness->runTestsFromFile(statusTests);
                break;
            case 6:
                testHarness->runTestsFromFile(stringTests);
                break;
            case 7:
                testHarness->runTestsFromFile(otherTests);
                break;
            case 8:
                std::cout << "Exiting...\n";
                delete testHarness;
                return 0;
            default:
                std::cout << "Invalid choice! Try again.\n";
                break;
        }
    }

    // Safety (though unreachable)
    delete testHarness;
    return 0;
}