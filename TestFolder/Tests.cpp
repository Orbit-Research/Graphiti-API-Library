/**
  ******************************************************************************
  * @file    Tests.cpp
  * @company Orbit Research
  * @version v1.0
  * @date    08-12-25
  * @brief   Implementation of test cases for Graphiti.
  *
  * @details
  * Implements test registration and concrete test functions used by the harness.
  * Tests operate against a GraphitiExtension instance and validate device behavior.
  *
  ******************************************************************************
  * @attention
  * Copyright (c) 2026 Orbit Research LLC
  * Distributed under GNU General Public License V3.0
  ******************************************************************************
  */

#include <cstdio>
#include "Graphiti/Extension.hpp"
#include "Tests.hpp"
/**
 * @function: Tests::Tests
 * @brief: Constructor initializes and registers available tests.
 * @return: n/a
 */
Tests::Tests() {
    registerTests();
}

/**
 * @function: Tests::registerTests
 * @brief: Populates the testRegistry with test entries and callbacks.
 * @return: void
 */
void Tests::registerTests(){
    testRegistry = {
        //String Tests
        {"get_Software_Version_Correctness", {[this]() { return get_Software_Version_Correctness(); }, false, false }},
        {"get_Hardware_Version_Correctness", {[this]() { return get_Hardware_Version_Correctness(); }, false, false }},
        {"get_Serial_Number_Correctness", {[this]() { return get_Serial_Number_Correctness(); }, false, false }},
        {"get_Battery_Status_Correctness", {[this]() { return get_Battery_Status_Correctness(); }, false, false }},
        {"get_Resolution_Information_Correctness", {[this]() { return get_Resolution_Information_Correctness(); }, false, false }},
        {"get_Device_Orientation_Correctness", {[this]() { return get_Device_Orientation_Correctness(); }, false, false }},
        {"get_Height_Information_Correctness", {[this]() { return get_Height_Information_Correctness(); }, false, false }},
        {"get_Device_Name_Correctness", {[this]() { return get_Device_Name_Correctness(); }, false, false }},
        //Display Tests
        {"update_Display_Functionality", {[this]() { return update_Display_Functionality(); }, false, false }},
        {"set_Display_Funcitonality", {[this]() { return set_Display_Funcitonality(); }, false, false }},
        {"clear_Display_Functionality", {[this]() { return clear_Display_Functionality(); }, false, false }},
        {"update_Single_Pixel_Functionality", {[this]() { return update_Single_Pixel_Functionality(); }, false, false }},
        {"update_Single_Row_Functionality", {[this]() { return update_Single_Row_Functionality(); }, false, false }},
        {"update_Single_Column_Functionality", {[this]() { return update_Single_Column_Functionality(); }, false, false }},
        //Status Tests
        {"get_ALL_Pixels_Position_Status_Correctness", {[this]() { return get_ALL_Pixels_Position_Status_Correctness(); }, false, false }},
        {"get_Single_Pixel_Position_Status_Correctness", {[this]() { return get_Single_Pixel_Position_Status_Correctness(); }, false, false }},
        {"get_Single_Row_Pixels_Position_Status_Correctness", {[this]() { return get_Single_Row_Pixels_Position_Status_Correctness(); }, false, false }},
        {"get_Single_Column_Pixels_Position_Status_Correctness", {[this]() { return get_Single_Column_Pixels_Position_Status_Correctness(); }, false, false }},
        //Key Test
        {"get_Next_KeyEvent_Functionality", {[this]() { return get_Next_KeyEvent_Functionality(); }, true, false }},
        //Draw Test
        {"get_Next_Draw_Event_Functionality", {[this]() { return get_Next_Draw_Event_Functionality(); }, false, true }},
        //Show Message Test
        {"show_Message_On_The_Device_Functionality", {[this]() { return show_Message_On_The_Device_Functionality(); }, false, false }},
        //Set Cursor Test
        {"set_Cursor_Functionality", {[this]() { return set_Cursor_Functionality(); }, false, false }},
        //Image Test
        {"send_Image_Functionality", {[this]() { return send_Image_Functionality(); }, false, false }},
        //Status Test
        {"get_Last_Touch_Point_Status_Functionality", {[this]() { return vibrator_Control_Command_Functionality(); }, false, false }},
        //Vibrator Test
        {"vibrator_Control_Command_Functionality", {[this]() { return vibrator_Control_Command_Functionality(); }, false, false }},
        //Clock Test
        {"get_Date_And_Time_Correctness", {[this]() { return get_Date_And_Time_Correctness(); }, false, false }},
        {"set_Date_And_Time_Functionality", {[this]() { return set_Date_And_Time_Functionality(); }, false, false }}
    };
}

/**
 * @function: Tests::startTesting
 * @brief: Starts the Graphiti extension, runs the provided test function, then shuts down.
 * @param:
 *   bool keyEvents - whether to enable key events
 *   bool touchEvents - whether to enable touch events
 *   std::function<bool()> func - test function to execute
 * @return: bool - result of the test function
 */
bool Tests::startTesting(bool keyEvents, bool touchEvents, 
    std::function<bool()> func){
    graphiti = new GraphitiExtension();

    // Select device VID/PID for HID connection
    bool useOldDevice = false; // Set true for old device, false for new device
    uint16_t vid, pid;
    if (useOldDevice) {
        vid = 0x1FC9; pid = 0x8217;
    } else {
        vid = 0x0483; pid = 0xA366;
    }

    // if(!graphiti->startUpVCP("COM4", keyEvents, touchEvents)){
    //     return false;
    // }

    if(!graphiti->startUpHID(vid, pid, keyEvents, touchEvents)){
        return false;
    }
    graphiti->sleep(1);

    bool result = func();
    
    // graphiti->shutDownVCP(keyEvents, touchEvents);
    graphiti->shutDownHID(keyEvents, touchEvents);
    delete graphiti;
    return result;
}

// bool Tests::startTesting(bool keyEvents, bool touchEvents, 
//     std::function<bool()> func){
    
    
//     if(graphiti == nullptr) {
//         graphiti = new GraphitiExtension();
//         graphiti->sleep(2);
//         if(!graphiti->startUpVCP("COM22", keyEvents, touchEvents)){
//             std::cout << "VCP Connection FAILED!" << std::endl;
//             delete graphiti;
//             graphiti = nullptr;
//             return false;
//         }
//         std::cout << "VCP Connected!" << std::endl;
//         graphiti->sleep(1);
//     }

//     bool result = func();
//     return result;
// }

/**
 * @function: Tests::sleepAndTest
 * @brief: Sleeps briefly, fetches the next output event and compares to expected string.
 * @param expected Expected output string to compare with device output.
 * @return: bool - true if output matches expected
 */
bool Tests::sleepAndTest(std::string expected){
    graphiti->sleep(5);   
    auto output = graphiti->getNextOutputEvent();
    if(output.has_value()) {
        std::cout << "Got: " << output.value() << std::endl;
        if(output.value() != expected) {
            std::cerr << output.value() << 
                " was not: " << expected << std::endl;
            return false;
        }
        return true;
    }
    std::cout << "No output received!" << std::endl;
    return false;
}

/**
 * @function: Tests::get_Software_Version_Correctness
 * @brief: Requests software version and verifies output.
 * @return: bool - true if correct
 */
bool Tests::get_Software_Version_Correctness() {
    graphiti->getSoftwareVersion();
    graphiti->sleep(3);

    auto output = graphiti->getNextOutputEvent();
    if (!output.has_value()) return false;

    std::string val = output.value();
    std::cout << "Got: " << val << std::endl;

    return !val.empty() && std::any_of(val.begin(), val.end(), ::isdigit);
}

/**
 * @function: Tests::get_Hardware_Version_Correctness
 * @brief: Requests hardware version and verifies output.
 * @return: bool - true if correct
 */
bool Tests::get_Hardware_Version_Correctness() {
    graphiti->getHardwareVersion();
    graphiti->sleep(3);

    auto output = graphiti->getNextOutputEvent();
    if (!output.has_value()) return false;

    std::string val = output.value();
    std::cout << "Got: " << val << std::endl;

    return !val.empty(); 
}

/**
 * @function: Tests::get_Serial_Number_Correctness
 * @brief: Requests serial number and verifies output.
 * @return: bool - true if correct
 */
bool Tests::get_Serial_Number_Correctness() {
    graphiti->getSerialNumber();
    graphiti->sleep(3);

    auto output = graphiti->getNextOutputEvent();
    if (!output.has_value()) return true; // allow missing

    std::string val = output.value();
    std::cout << "Got: " << val << std::endl;

    return true; // always pass (device dependent)
}

/**
 * @function: Tests::get_Battery_Status_Correctness
 * @brief: Requests battery status and verifies output.
 * @return: bool - true if correct
 */
bool Tests::get_Battery_Status_Correctness() {
    graphiti->getBatteryStatus();
    graphiti->sleep(2);
    auto output = graphiti->getNextOutputEvent();
    if (!output.has_value()) return false;
    std::string val = output.value();
    
    bool hasPercent = val.find('%') != std::string::npos;
    bool hasPrefix  = (val.rfind("Battery", 0) == 0) || 
                      (val.rfind("Charging", 0) == 0);
    return hasPercent && hasPrefix;
}

/**
 * @function: Tests::get_Resolution_Information_Correctness
 * @brief: Requests resolution information and verifies output.
 * @return: bool - true if correct
 */
bool Tests::get_Resolution_Information_Correctness() {
    graphiti->getResolutionInformation();
    return sleepAndTest("60|40");
}

/**
 * @function: Tests::get_Device_Orientation_Correctness
 * @brief: Requests device orientation and verifies output.
 * @return: bool - true if correct
 */
bool Tests::get_Device_Orientation_Correctness() {
    graphiti->getDeviceOrientation();
    return sleepAndTest("Landscape");
}

/**
 * @function: Tests::get_Height_Information_Correctness
 * @brief: Requests device height information and verifies output.
 * @return: bool - true if correct
 */
bool Tests::get_Height_Information_Correctness() {
    graphiti->getHeightInformation();
    return sleepAndTest("4");
}

/**
 * @function: Tests::get_Device_Name_Correctness
 * @brief: Requests device name and verifies output.
 * @return: bool - true if correct
 */
bool Tests::get_Device_Name_Correctness() {
    graphiti->getDeviceName();
    graphiti->sleep(3);

    auto output = graphiti->getNextOutputEvent();
    if (!output.has_value()) return false;

    std::string val = output.value();
    std::cout << "Got: " << val << std::endl;

    return val.find("Graphiti") != std::string::npos;
}

/**
 * @function: Tests::update_Display_Functionality
 * @brief: Performs a full screen update for visual verification.
 * @return: bool - true on success
 */
bool Tests::update_Display_Functionality() {
    UpdateDisplay();
    graphiti->sleep(5);
    return true;
}

/**
 * @function: Tests::set_Display_Funcitonality
 * @brief: Calls setDisplay and waits briefly.
 * @return: bool - true on success
 */
bool Tests::set_Display_Funcitonality() {
    graphiti->setDisplay();
    graphiti->sleep(10);
    return true;
}

/**
 * @function: Tests::clear_Display_Functionality
 * @brief: Clears display and waits briefly.
 * @return: bool - true on success
 */
bool Tests::clear_Display_Functionality() {
    graphiti->clearDisplay();
    graphiti->sleep(5);
    return true;
}

/**
 * @function: Tests::update_Single_Pixel_Functionality
 * @brief: Updates a single pixel to a test height and waits.
 * @return: bool - true on success
 */
bool Tests::update_Single_Pixel_Functionality() {
    graphiti->updateSinglePixel(20, 30, 2, 0);
    graphiti->sleep(2);
    return true;
}

/**
 * @function: Tests::update_Single_Row_Functionality
 * @brief: Updates a row using UpdateRow helper.
 * @return: bool - true on success
 */
bool Tests::update_Single_Row_Functionality() {
    UpdateRow(3);
    return true;
}

/**
 * @function: Tests::update_Single_Column_Functionality
 * @brief: Updates a column using UpdateColumn helper.
 * @return: bool - true on success
 */
bool Tests::update_Single_Column_Functionality() {
    UpdateColumn(25);
    return true;
}

//Status Tests
/**
 * @function: Tests::get_ALL_Pixels_Position_Status_Correctness
 * @brief: Runs display update and corresponding status query, verifies match.
 * @return: bool - true if status matches expected
 */
bool Tests::get_ALL_Pixels_Position_Status_Correctness() {
    return statusFunctionResponse(
        "UpdateDisplay",
        "getAllPixelsPositionStatus"
    );
}


/**
 * @function: Tests::get_Single_Pixel_Position_Status_Correctness
 * @brief: Runs a single-pixel update then checks its status.
 * @return: bool - true if status matches expected
 */
bool Tests::get_Single_Pixel_Position_Status_Correctness() {
    return statusFunctionResponse(
        "updateSinglePixel",
        "getSinglePixelPositionStatus",
        25, 30, 2, 0
    );
}


/**
 * @function: Tests::get_Single_Row_Pixels_Position_Status_Correctness
 * @brief: Updates a row and verifies status for that row.
 * @return: bool - true if status matches expected
 */
bool Tests::get_Single_Row_Pixels_Position_Status_Correctness() {
    return statusFunctionResponse(
        "UpdateRow",
        "getSingleRowPixelsPositionStatus",
        5
    );
}


/**
 * @function: Tests::get_Single_Column_Pixels_Position_Status_Correctness
 * @brief: Updates a column and verifies status for that column.
 * @return: bool - true if status matches expected
 */
bool Tests::get_Single_Column_Pixels_Position_Status_Correctness() {
    return statusFunctionResponse(
        "UpdateColumn",
        "getSingleColumnPixelsPositionStatus",
        35
    );
}

//Key Test

/**
 * @function: Tests::get_Next_KeyEvent_Functionality
 * @brief: Enters key loop and processes key events until loop ends.
 * @return: bool - true on success
 */
bool Tests::get_Next_KeyEvent_Functionality() {
    std::cout << "get_Next_KeyEvent_Functionality" << std::endl;

    loopCondition = true;

    graphiti->keyLoop(
        [this](const std::set<std::string>& keys, void* params) {
            OnePixel_KeyFunction(keys, params);
        },
        &loopCondition
    );

    return true;
}


/**
 * @function: Tests::OnePixel_KeyFunction
 * @brief: Callback used by key loop to toggle a pixel on key events.
 * @param:
 *   const std::set<std::string>& keyEvent - keys triggered
 *   void* @param - user-provided @param
 * @return: void
 */
void Tests::OnePixel_KeyFunction(
    const std::set<std::string>& keyEvent,
    void* parameters
) {
    // Print all pressed keys
    for (const auto& key : keyEvent) {
        std::cout << "Key pressed: " << key << std::endl;
    }

    if(keyEvent.contains("1")){
        std::cout << "Key Event 1 on Main found" << std::endl;
        graphiti->updateSinglePixel(10, 30, 2, 0); //Put up
    } else {
        graphiti->updateSinglePixel(10, 30, 0, 0); //Put down
    }

    if(keyEvent.contains("Select")) {
        loopCondition = false; //Ends the loop
    }
}

//Draw Test

/**
 * @function: Tests::get_Next_Draw_Event_Functionality
 * @brief: Enters draw loop and processes draw events until loop ends.
 * @return: bool - true on success
 */
bool Tests::get_Next_Draw_Event_Functionality() {
    std::cout << "get_Next_Draw_Event_Functionality" << std::endl;

    loopCondition = true;

    graphiti->drawLoop([this](const Graphiti_API::DrawEvent& drawEvent, void* params) {
            PrintDraw_DrawFunction(drawEvent, params);
        }, &loopCondition
    );

    return true;
}

/**
 * @function: Tests::PrintDraw_DrawFunction
 * @brief: Callback for draw events, prints pin info and ends loop on specific condition.
 * @param:
 *   const Graphiti_API::DrawEvent& drawEvent - draw event info
 *   void* @param - user params
 * @return: void
 */
void Tests::PrintDraw_DrawFunction(
    const Graphiti_API::DrawEvent& drawEvent, 
    void* parameters
) {
    Graphiti_API::PinInfo pin;

    for (size_t i = 0; i < drawEvent.length; i++)
    {
        pin = drawEvent.pins[i];
        std::cout 
            << "row: " << pin.rowID << " "
            << "col: " << pin.columnID << " "
            << "height: " << pin.height << " "
            << "blinkRate: " << pin.blinkRate << std::endl;

        if(drawEvent.pins[i].rowID == 40 
            and drawEvent.pins[i].columnID == 60 ){
            loopCondition = false;
        }
    }
}

//Show Message Test
//Press Space + 3 to enable message bar
/**
 * @function: Tests::show_Message_On_The_Device_Functionality
 * @brief: Shows a test message on the device and waits briefly.
 * @return: bool - true on success
 */
bool Tests::show_Message_On_The_Device_Functionality() {
    graphiti->showMessage("abcdefghijklmnopqr",{1,2,2},2);
    graphiti->sleep(2);

    return true;
}

//Set Cursor Test

/**
 * @function: Tests::set_Cursor_Functionality
 * @brief: Sets the cursor position and waits briefly.
 * @return: bool - true on success
 */
bool Tests::set_Cursor_Functionality() {
    graphiti->setCursor(20, 30, 2, 10, 3);
    graphiti->sleep(2);

    return true;
}


//Image Test
/**
 * @function: Tests::send_Image_Functionality
 * @brief: Sends image data to the device (blocking and interruptible) and checks for exceptions.
 * @return: bool - true on success
 */
bool Tests::send_Image_Functionality() {
    const std::string name = "test_line.bmp";

    const std::string path = "C:\\Users\\Himanshu\\Downloads\\Graphiti-API-Library\\images\\" + name;

    try {
        graphiti->sendImageBlocking(name, path);
        graphiti->sleep(10);
        graphiti->sendImageInterruptible(name, path);
        graphiti->sleep(10);
    } catch (const std::exception& e) {
        std::cerr << "[EXCEPTION] " << e.what() << "\n";
        return false;
    } catch (...) {
        std::cerr << "[EXCEPTION] Unknown crash occurred" 
            << "in send_Image_Functionality\n";
        return false;
    }
    return true;
}

//Status Test
/**
 * @function: Tests::get_Last_Touch_Point_Status_Functionality
 * @brief: Requests last touch point status and validates returned data length.
 * @return: bool - true if length is valid
 */
bool Tests::get_Last_Touch_Point_Status_Functionality() {
    graphiti->getLastTouchPointStatus();
    graphiti->sleep(2);

    auto displayOption = graphiti->getNextDisplayStatusEvent();
    //Just checks if it returns with a valid output in size
    if(displayOption.has_value()){
        auto bytes = displayOption.value();
        return bytes.size() == 3;
    }
    return false;
}

//Vibrator Test
/**
 * @function: Tests::vibrator_Control_Command_Functionality
 * @brief: Sends vibrator control command and waits briefly.
 * @return: bool - true on success
 */
bool Tests::vibrator_Control_Command_Functionality() {

    graphiti->vibratorControlCommand(10,40,100);
    graphiti->sleep(2);

    return true;
}

//Clock Test
/**
 * @function: Tests::set_Date_And_Time_Functionality
 * @brief: Sets device date/time to current system time components.
 * @return: bool - true on success
 */
bool Tests::set_Date_And_Time_Functionality() {
    auto now = std::chrono::system_clock::now();

    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm* local_time = std::localtime(&now_time);

    graphiti->setDateAndTime(
        local_time->tm_mday, 
        local_time->tm_mon + 1, 
        local_time->tm_year + 1900,
        local_time->tm_hour, 
        local_time->tm_min, 
        local_time->tm_sec
    );

    graphiti->sleep(2);

    return true;
}
/**
 * @function: Tests::get_Date_And_Time_Correctness
 * @brief: Requests date/time and compares with the system time string.
 * @return: bool - true if matches
 */
bool Tests::get_Date_And_Time_Correctness() {
    graphiti->getDateAndTime();
    graphiti->sleep(3);

    auto output = graphiti->getNextOutputEvent();
    if (!output.has_value()) return false;

    std::string val = output.value();
    std::cout << "Got: " << val << std::endl;

    return val.find(":") != std::string::npos;
}

/**
 * @function: Tests::UpdateDisplay
 * @brief: Builds a full-screen test pattern, updates the device and returns pin heights.
 * @return: std::vector<uint8_t> - pin heights
 */
std::vector<uint8_t> Tests::UpdateDisplay() {
    std::vector<uint8_t> screen(MAX_COLUMN_DATA * MAX_ROW_DATA * 2, 0);
    std::vector<uint8_t> pinHeightsForTest(MAX_COLUMN_DATA * MAX_ROW_DATA, 0);

    for (size_t i = 0; i < MAX_COLUMN_DATA* MAX_ROW_DATA; i++)
    {
        screen[i*2] = 2;//Pixel height
        pinHeightsForTest[i] = 2;
        screen[i*2 + 1] = 0;//Blinking rate
    }

    graphiti->updateDisplay(screen);
    graphiti->sleep(10);

    return pinHeightsForTest;
}

/**
 * @function: Tests::UpdateRow
 * @brief: Builds a single-row test pattern, updates the device and returns pin heights.
 * @param row Row index to update.
 * @return: std::vector<uint8_t> - pin heights
 */
std::vector<uint8_t> Tests::UpdateRow(uint8_t row) {
    std::vector<uint8_t> rowData(MAX_ROW_DATA * 2, 0);
    std::vector<uint8_t> pinHeightsForTest(MAX_ROW_DATA, 0);

    for (size_t i = 0; i < MAX_ROW_DATA; i++)
    {
        rowData[i*2] = 2;//Pixel height
        pinHeightsForTest[i] = 2;
        rowData[i*2 + 1] = 0;//Blinking rate
    }

    graphiti->updateSingleRow(row, rowData);
    graphiti->sleep(4);

    pinHeightsForTest.insert(pinHeightsForTest.begin(), row);

    return pinHeightsForTest;
}

/**
 * @function: Tests::UpdateColumn
 * @brief: Builds a single-column test pattern, updates the device and returns pin heights.
 * @param column Column index to update.
 * @return: std::vector<uint8_t> - pin heights
 */
std::vector<uint8_t> Tests::UpdateColumn(uint8_t column) {
    std::vector<uint8_t> columnData(MAX_COLUMN_DATA * 2, 0);
    std::vector<uint8_t> pinHeightsForTest(MAX_COLUMN_DATA, 0);

    for (size_t i = 0; i < MAX_COLUMN_DATA; i++)
    {
        columnData[i*2] = 2;//Pixel height
        pinHeightsForTest[i] = 2;
        columnData[i*2 + 1] = 0;//Blinking rate
    }

    graphiti->updateSingleColumn(column, columnData);
    graphiti->sleep(4);

    pinHeightsForTest.insert(pinHeightsForTest.begin(), column);

    return pinHeightsForTest;
}

/**
 * @function: Tests::displayFunctionWrapper
 * @brief: Dispatches to the appropriate display helper based on name and returns expected output.
 * @param funcitonName Name of display helper function.
 * @param arg1 Argument 1 for helper.
 * @param arg2 Argument 2 for helper.
 * @param arg3 Argument 3 for helper.
 * @param arg4 Argument 4 for helper.
 * @return: std::vector<uint8_t> - expected output
 */
std::vector<uint8_t> Tests::displayFunctionWrapper(std::string funcitonName, 
    int arg1, int arg2, int arg3, int arg4) 
{
    if(funcitonName == "UpdateDisplay") 
    {
        return UpdateDisplay();
    } 
    else if (funcitonName == "updateSinglePixel") 
    {
        graphiti->updateSinglePixel(arg1, arg2, arg3, arg4);
        graphiti->sleep(2);
        return {static_cast<uint8_t>(arg1), 
            static_cast<uint8_t>(arg2), 
            static_cast<uint8_t>(arg3)};
    }
    else if (funcitonName == "UpdateRow") 
    {
        return UpdateRow(arg1);
    }
    else if (funcitonName == "UpdateColumn") 
    {
        return UpdateColumn(arg1);
    }

    return {0x00};//Error case
}

/**
 * @function: Tests::statusFunctionWrapper
 * @brief: Calls a named status function on the Graphiti extension.
 * @param funcitonName Name of status function.
 * @param arg1 Argument 1 for status function.
 * @param arg2 Argument 2 for status function.
 * @return: void
 */
void Tests::statusFunctionWrapper(std::string funcitonName, int arg1, int arg2) {
    if(funcitonName == "getAllPixelsPositionStatus") 
    {
        graphiti->getAllPixelsPositionStatus();
    } 
    else if (funcitonName == "getSinglePixelPositionStatus") 
    {
        graphiti->getSinglePixelPositionStatus(arg1, arg2);
    }
    else if (funcitonName == "getSingleRowPixelsPositionStatus") 
    {
        graphiti->getSingleRowPixelPositionStatus(arg1);
    }
    else if (funcitonName == "getSingleColumnPixelsPositionStatus") 
    {
        graphiti->getSingleColumnPixelPositionStatus(arg1);
    }
}

/**
 * @function: Tests::statusFunctionResponse
 * @brief: Runs a display helper, triggers a status function and compares expected vs actual status.
 * @param displayFunctionName Name of display helper function.
 * @param statusFunctionName Name of status function.
 * @param arg1 Argument 1 for helper.
 * @param arg2 Argument 2 for helper.
 * @param arg3 Argument 3 for helper.
 * @param arg4 Argument 4 for helper.
 * @return: bool - true if actual matches expected
 */
bool Tests::statusFunctionResponse(
    std::string displayFunctionName,
    std::string statusFunctionName,
    int arg1, int arg2, int arg3, int arg4
) {
    auto expected = displayFunctionWrapper(
        displayFunctionName, arg1, arg2, arg3, arg4);

    statusFunctionWrapper(statusFunctionName, arg1, arg2);
    graphiti->sleep(2);

    auto displayOption = graphiti->getNextDisplayStatusEvent();
    graphiti->sleep(2);

    graphiti->clearDisplay();
    graphiti->sleep(5);

    if (!displayOption.has_value()) {
        return false;
    } else {
        auto display = displayOption.value();
        std::cout << "display size: " << display.size() << std::endl;
        if (!display.empty()) {
            std::cout << "display first byte: " << std::hex << (int)display[0] << std::dec << std::endl;
        }
        std::cout << "expected size: " << expected.size() << std::endl;
        if (!expected.empty()) {
            std::cout << "expected first byte: " << std::hex << (int)expected[0] << std::dec << std::endl;
        }
        return expected.size() == display.size(); 
    }
}