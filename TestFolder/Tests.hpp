/** ******************************************************************************
 * @file    Tests.hpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   Tests class declarations for the test harness.
 *
 * @details
 * Declares the `Tests` class, test entry registry and helper methods used by
 * the `TestHarness` to run and evaluate Graphiti device tests (display, key,
 * draw, status and utility tests).
 *
 *******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/
#pragma once
#include <cstdio>
#include <chrono>
#include <Graphiti/Extension.hpp>

class Tests {
public:
    Tests();

    GraphitiExtension* graphiti = nullptr;

    struct TestEntry {
        std::function<bool()> func;
        bool keyEvents;
        bool touchEvents;
    };

    std::unordered_map<std::string, TestEntry> testRegistry;

    void registerTests();

    /**
     * @function: startTesting
     * @brief: Initialize Graphiti extension, run provided test function, then shutdown.
     * @Param:
     *   bool keyEvents - enable key events
     *   bool touchEvents - enable touch events
     *   std::function<bool()> func - test function to execute
     * @Return:
     *   bool - result of the test function
     */
    
    bool startTesting(bool keyEvents, 
        bool touchEvents, 
        std::function<bool()> func);

    /**
     * @function: get_Software_Version_Correctness
     * @brief: Requests software version and verifies expected output.
     * @return: bool - true if correct
     */
    bool get_Software_Version_Correctness();

    /**
     * @function: get_Hardware_Version_Correctness
     * @brief: Requests hardware version and verifies expected output.
     * @return: bool - true if correct
     */
    bool get_Hardware_Version_Correctness();

    /**
     * @function: get_Serial_Number_Correctness
     * @brief: Requests serial number and verifies expected output.
     * @return: bool - true if correct
     */
    bool get_Serial_Number_Correctness();

    /**
     * @function: get_Battery_Status_Correctness
     * @brief: Requests battery status and verifies expected output.
     * @return: bool - true if correct
     */
    bool get_Battery_Status_Correctness();

    /**
     * @function: get_Resolution_Information_Correctness
     * @brief: Requests resolution information and verifies expected output.
     * @return: bool - true if correct
     */
    bool get_Resolution_Information_Correctness();

    /**
     * @function: get_Device_Orientation_Correctness
     * @brief: Requests device orientation and verifies expected output.
     * @return: bool - true if correct
     */
    bool get_Device_Orientation_Correctness();

    /**
     * @function: get_Height_Information_Correctness
     * @brief: Requests device height information and verifies expected output.
     * @return: bool - true if correct
     */
    bool get_Height_Information_Correctness();

    /**
     * @function: get_Device_Name_Correctness
     * @brief: Requests device name and verifies expected output.
     * @return: bool - true if correct
     */
    bool get_Device_Name_Correctness();

    //Display Tests

    /**
     * @function: update_Display_Functionality
     * @brief: Performs display update test (visual verification required).
     * @return: bool - true on success
     */
    bool update_Display_Functionality();

    /**
     * @function: set_Display_Funcitonality
     * @brief: Calls setDisplay on device.
     * @return: bool - true on success
     */
    bool set_Display_Funcitonality();

    /**
     * @function: clear_Display_Functionality
     * @brief: Calls clearDisplay on device.
     * @return: bool - true on success
     */
    bool clear_Display_Functionality();

    /**
     * @function: update_Single_Pixel_Functionality
     * @brief: Updates a single pixel on the device.
     * @return: bool - true on success
     */
    bool update_Single_Pixel_Functionality();

    /**
     * @function: update_Single_Row_Functionality
     * @brief: Updates a single row on the device.
     * @return: bool - true on success
     */
    bool update_Single_Row_Functionality();

    /**
     * @function: update_Single_Column_Functionality
     * @brief: Updates a single column on the device.
     * @return: bool - true on success
     */
    bool update_Single_Column_Functionality();

    //Status Tests

    /**
     * @function: get_ALL_Pixels_Position_Status_Correctness
     * @brief: Verifies status for all pixel positions.
     * @return: bool - true if correct
     */
    bool get_ALL_Pixels_Position_Status_Correctness();

    /**
     * @function: get_Single_Pixel_Position_Status_Correctness
     * @brief: Verifies status for a single pixel position.
     * @return: bool - true if correct
     */
    bool get_Single_Pixel_Position_Status_Correctness();

    /**
     * @function: get_Single_Row_Pixels_Position_Status_Correctness
     * @brief: Verifies status for a single row of pixels.
     * @return: bool - true if correct
     */
    bool get_Single_Row_Pixels_Position_Status_Correctness();

    /**
     * @function: get_Single_Column_Pixels_Position_Status_Correctness
     * @brief: Verifies status for a single column of pixels.
     * @return: bool - true if correct
     */
    bool get_Single_Column_Pixels_Position_Status_Correctness();

    //Key Test

    /**
     * @function: get_Next_KeyEvent_Functionality
     * @brief: Enters key loop and handles key events for testing.
     * @return: bool - true on success
     */
    bool get_Next_KeyEvent_Functionality();

    /**
     * @function: OnePixel_KeyFunction
     * @brief: Key loop callback handling single-pixel updates based on key events.
     * @param:
     *   const std::set<std::string>& keyEvent - set of key identifiers
     *   void* @param - user @param passed to the callback
     * @return: void
     */
    
    void OnePixel_KeyFunction(
        const std::set<std::string>& keyEvent,
        void* parameters
    );

    //Draw Test

    /**
     * @function: get_Next_Draw_Event_Functionality
     * @brief: Enters draw loop and handles draw events for testing.
     * @return: bool - true on success
     */
    bool get_Next_Draw_Event_Functionality();

    /**
     * @function: PrintDraw_DrawFunction
     * @brief: Draw loop callback that prints draw event pin info.
     * @param:
     *   const Graphiti_API::DrawEvent& drawEvent - draw event data
     *   void* @param - user @param
     * @@return: void
     */
    
    void PrintDraw_DrawFunction(
        const Graphiti_API::DrawEvent& drawEvent, 
        void* parameters
    );

    /**
     * @function: show_Message_On_The_Device_Functionality
     * @brief: Shows a test message on the device.
     * @return: bool - true on success
     */
    bool show_Message_On_The_Device_Functionality();

    //Set Cursor Test
    /**
     * @function: set_Cursor_Functionality
     * @brief: Tests setting cursor on the device.
     * @return: bool - true on success
     */
    bool set_Cursor_Functionality();

    //Image Test
    /**
     * @function: send_Image_Functionality
     * @brief: Sends an image to the device for testing.
     * @return: bool - true on success
     */
    bool send_Image_Functionality();

    //Status Test
    /**
     * @function: get_Last_Touch_Point_Status_Functionality
     * @brief: Requests last touch point status and verifies output.
     * @param expectedStatus Expected status string for validation.
     * @return: bool - true if correct
     */
    bool get_Last_Touch_Point_Status_Functionality();

    //Vibrator Test
    /**
     * @function: vibrator_Control_Command_Functionality
     * @brief: Sends vibrator control command for testing.
     * @param command Command string defining vibrator behavior.
     * @return: bool - true on success
     */
    bool vibrator_Control_Command_Functionality();

    //Clock Test
    /**
     * @function: get_Date_And_Time_Correctness
     * @brief: Requests date/time and verifies against system time.
     *  @param dateAndTime Expected date and time string.
     * @return: bool - true if correct
     */
    bool get_Date_And_Time_Correctness();
    /**
     * @function: set_Date_And_Time_Functionality
     * @brief: Sets device date/time to current system time.
     * @param dateAndTime Date and time string to be set on the device.
     * @return: bool - true on success
     */
    bool set_Date_And_Time_Functionality();

private:
    
    bool loopCondition = true;

    /**
     * @function: sleepAndTest
     * @brief: Sleeps briefly, fetches next output event and compares to expected.
     * @param expected Expected output string to compare with device output.
     * @return: bool - true if output matches expected
     */
    bool sleepAndTest(std::string expected);
    /**
     * @function: UpdateDisplay
     * @brief: Helper that updates the full screen to a test pattern.
     * @return: std::vector<uint8_t> - pin heights set on screen
     */
    std::vector<uint8_t> UpdateDisplay();

    /**
     * @function: UpdateRow
     * @brief: Helper that updates a single row to a test pattern.
     * @param: uint8_t row - row index to update
     * @return: std::vector<uint8_t> - pin heights for the row
     */
    std::vector<uint8_t> UpdateRow(uint8_t row);


    /**
     * @function: UpdateColumn
     * @brief: Helper that updates a single column to a test pattern.
     * @param: uint8_t column - column index to update
     * @return: std::vector<uint8_t> - pin heights for the column
     */
    std::vector<uint8_t> UpdateColumn(uint8_t column);

    /**
     * @function: displayFunctionWrapper
     * @brief: Wrapper that calls various display helper functions by name.
     * @param: std::string funcitonName - name of helper to call
     *             int arg1..arg4 - helper arguments
     * @return: std::vector<uint8_t> - expected display output
     */
    std::vector<uint8_t> displayFunctionWrapper(
        std::string funcitonName, 
        int arg1, int arg2, int arg3, int arg4);

    /**
     * @function: statusFunctionWrapper
     * @brief: Wrapper that calls status query functions by name.
     * @param: std::string funcitonName - name of status function
     *             int arg1, int arg2 - arguments for status function
     * @return: void
     */
    void statusFunctionWrapper(
        std::string funcitonName, int arg1, int arg2);

    /**
     * @function: statusFunctionResponse
     * @brief: Executes a display helper then corresponding status query and compares results.
     * @Params: std::string displayFunctionName - display helper name
     *             std::string statusFunctionName - status function name
     *             int arg1..arg4 - helper arguments
     * @return: bool - true if status matches expected
     */
    bool statusFunctionResponse(
        std::string displayFunctionName,
        std::string statusFunctionName,
        int arg1 = 0, int arg2 = 0, int arg3 = 0, int arg4 = 0
    );
};