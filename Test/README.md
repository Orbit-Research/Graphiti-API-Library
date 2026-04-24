## Overview

This project is a **Test Harness** that connects to a Graphiti device and automatically tests its hardware and software functionalities. It includes tests across display, draw, image, key, status, string, and other categories.


## File Structure

Test/
├── main.cpp              → Interactive CLI test runner
├── TestHarness.hpp       → Test Harness class header
├── TestHarness.cpp       → Test Harness function implementations
├── Tests.hpp             → Tests class declarations
├── Tests.cpp             → All test implementations
└── TestData/
    ├── Tests.txt         → Full list of all tests
    ├── DisplayTests.txt  → Display category tests
    ├── DrawTests.txt     → Draw category tests
    ├── ImageTests.txt    → Image category tests
    ├── KeyTests.txt      → Key event category tests
    ├── ManualTests.txt   → Manual tests (Draw + Key)
    ├── StatusTests.txt   → Pixel status category tests
    ├── StringTests.txt   → String/version info tests
    ├── OtherTests.txt    → Miscellaneous tests
    ├── passedTests.txt   → Passed tests from the last run
    └── failedTests.txt   → Failed tests from the last run


## Test Categories & Test List

### 1. String Tests (`StringTests.txt`)
Verifies string responses returned from the device.

 `get_Software_Version_Correctness` | Fetches software version and verifies against expected output |
 `get_Hardware_Version_Correctness` | Verifies hardware version string |
 `get_Serial_Number_Correctness` | Checks serial number correctness |
 `get_Battery_Status_Correctness` | Verifies battery status string |
 `get_Resolution_Information_Correctness` | Verifies resolution info string |
 `get_Device_Orientation_Correctness` | Verifies device orientation string |
 `get_Height_Information_Correctness` | Verifies device height info string |
 `get_Device_Name_Correctness` | Verifies device name string |

---

### 2. Display Tests (`DisplayTests.txt`)
Tests display update and pixel control functionality.

 `update_Display_Functionality` | Performs a full display update (visual verification required) |
 `set_Display_Funcitonality` | Calls the `setDisplay` command on the device |
 `clear_Display_Functionality` | Calls the `clearDisplay` command on the device |
 `update_Single_Pixel_Functionality` | Updates a single pixel on the device |
 `update_Single_Row_Functionality` | Updates a single row on the device |
 `update_Single_Column_Functionality` | Updates a single column on the device |

---

### 3. Status Tests (`StatusTests.txt`)
Verifies pixel position status correctness.

 `get_ALL_Pixels_Position_Status_Correctness` | Verifies status for all pixel positions |
 `get_Single_Pixel_Position_Status_Correctness` | Verifies status for a single pixel position |
 `get_Single_Row_Pixels_Position_Status_Correctness` | Verifies status for a single row of pixels |
 `get_Single_Column_Pixels_Position_Status_Correctness` | Verifies status for a single column of pixels |


### 4. Key Tests (`KeyTests.txt`)
Tests hardware key events.

 `get_Next_KeyEvent_Functionality` | Enters the key event loop and handles key events for testing |

>  **Note:** Press the **Select button** to exit the key test.



### 5. Draw Tests (`DrawTests.txt`)
Tests touch/draw events.

 `get_Next_Draw_Event_Functionality` | Enters the draw event loop and handles draw events for testing |

>  **Note:** Draw into the **bottom-right corner** of the Graphiti unit to exit the draw test.  
>  The device must be in **Draw Mode** for the draw test to execute properly.


### 6. Image Tests (`ImageTests.txt`)


 `send_Image_Functionality` | Sends an image to the device and verifies it |

---

### 7. Other Tests (`OtherTests.txt`)
Miscellaneous functionality tests.

 `show_Message_On_The_Device_Functionality` | Displays a test message on the device |
 `set_Cursor_Functionality` | Tests setting the cursor on the device |
 `get_Last_Touch_Point_Status_Functionality` | Requests last touch point status and verifies output |
 `vibrator_Control_Command_Functionality` | Sends a vibrator control command to the device |
 `get_Date_And_Time_Correctness` | Fetches device date/time and verifies against system time |
 `set_Date_And_Time_Functionality` | Sets the device date/time to the current system time |


### 8. Manual Tests (`ManualTests.txt`)
These tests require manual interaction with the device:
- `get_Next_Draw_Event_Functionality`
- `get_Next_KeyEvent_Functionality`



## Build & Run

### Build Commands (PowerShell)

```powershell
# Standard build
.\scripts\tools.ps1 standard

# Debug build
.\scripts\tools.ps1 standard_debug
```

> **Note:** Build scripts use **Ninja**. If Ninja is not available, use the equivalent commands provided in `scripts/tools.ps1`.

---

## Running Tests

### Interactive Menu (`main.cpp`)
Running the program presents a CLI menu:

```
Select Test Suite
1. displayTests
2. drawTests
3. imageTests
4. keyTests
5. statusTests
6. stringTests
7. otherTests
8. Exit
```

### Running Tests from a File
```cpp
testHarness->runTestsFromFile(displayTests);   // Runs DisplayTests.txt
testHarness->runTestsFromFile(passedTests);    // Runs passedTests.txt
```

### Running a Single Test
```cpp
testHarness->runTest("clear_Display_Functionality");
```

---

## Connection Mode (VCP / HID)

Tests can be run in either VCP or HID mode. In `Tests.cpp`, inside the `startTesting` function:

- **HID mode:** Use `startUpHID` / `shutDownHID`
- **VCP mode:** Use `startUpVCP` / `shutDownVCP` and set the correct **COM port**

---

## Test Results

After every test run, results are automatically saved to:

- `TestData/passedTests.txt` — Passed tests (current + previous runs)
- `TestData/failedTests.txt` — Failed tests (current + previous runs)

---

## Important Notes

1. The device must be in **Draw Mode** before running the draw test.
2. To exit the key test, press the **Select button**.
3. To exit the draw test, draw into the **bottom-right corner** of the Graphiti unit.
4. **Ninja** is required for the build scripts (or use equivalent commands from `scripts/tools.ps1`).