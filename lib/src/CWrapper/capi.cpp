/** ******************************************************************************
 * @file    capi.cpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   C wrapper implementation for Graphiti library.
 *
 * @details
 * Implements a C-compatible wrapper exposing a minimal set of Graphiti
 * functionality for use by language bindings. Keeps ABI-stable C interface
 * while delegating to the C++ core.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/

#include <vector>
#include <set>
#include <string>
#include <cstring>

#include <Graphiti/CWrapper/capi.h>
#include <Graphiti/Extension.hpp>

// Opaque handle = C++ class
struct GraphitiHandle {
    GraphitiExtension api;
};

// Constructor/Destructor
extern "C" {
    /**
     * @function graphiti_create
     * @brief Allocate and return a new GraphitiHandle (C wrapper).
     * @return GraphitiHandle* Pointer to new handle.
     */
    GraphitiHandle* graphiti_create() {
        printf("[C API] graphiti_create entered\n");
        fflush(stdout);
        GraphitiHandle* h = nullptr;
        try {
            h = new GraphitiHandle();
        } catch (const std::exception &e) {
            fprintf(stderr, "[C API] graphiti_create failed exception: %s\n", e.what());
            return nullptr;
        } catch (...) {
            fprintf(stderr, "[C API] graphiti_create failed unknown exception\n");
            return nullptr;
        }
        printf("[C API] graphiti_create returning %p\n", (void*)h);
        fflush(stdout);
        return h;
    }
        /**
         * @function: graphiti_createWithConnection
         * @brief: Create handle using existing GraphitiConnection pointer.
         * @param: void* connection - pointer to GraphitiConnection
         * @return: GraphitiHandle*
         */
    GraphitiHandle* graphiti_createWithConnection(void* connection) {
        return new GraphitiHandle{static_cast<GraphitiConnection*>(connection)};
    }
    /**
     * @function graphiti_destroy
     * @brief Destroys and frees a Graphiti handle.
     * @param handle Graphiti handle to destroy.
     */
    void graphiti_destroy(GraphitiHandle* handle) {
        delete handle;
    }

    /**
    * @function graphiti_setConnection
    * @brief Sets a native connection object for the Graphiti handle.
    *
    * @param handle Graphiti handle
    * @param connection Pointer to GraphitiConnection
    */
    void graphiti_setConnection(GraphitiHandle* handle, void* connection) {
        handle->api.setConnection(static_cast<GraphitiConnection*>(connection));
    }
    /**
    * @function graphiti_startUpVCP
    * @brief Starts communication with the device using VCP (serial).
    *
    * @param handle Graphiti handle
    * @param portName Serial port name
    * @param keyEventsBool Enable key events
    * @param touchEventsBool Enable touch events
    * @return bool True on successful connection
    */
    bool graphiti_startUpVCP(GraphitiHandle* handle, const char* portName, 
        bool keyEventsBool, bool touchEventsBool
    ){
        return handle->api.startUpVCP(portName, keyEventsBool, touchEventsBool);
    }
    /**
     * @function graphiti_shutDownVCP
     * @brief Shuts down the VCP connection.
     * @param handle Graphiti handle.
     * @param keyEventsBool Disable key events.
     * @param touchEventsBool Disable touch events.
     */
    void graphiti_shutDownVCP(GraphitiHandle* handle, 
        bool keyEventsBool, bool touchEventsBool
    ){
        handle->api.shutDownVCP(keyEventsBool, touchEventsBool);
    }
    /**
     * @function graphiti_startUpHID
     * @brief Starts communication with the device using HID.
     *
     * @param handle Graphiti handle
     * @param vendor_id USB vendor ID
     * @param product_id USB product ID
     * @param keyEventsBool Enable key events
     * @param touchEventsBool Enable touch events
     * @return bool True on successful connection
     */
    bool graphiti_startUpHID(GraphitiHandle* handle, uint16_t vendor_id, uint16_t product_id, 
        bool keyEventsBool, bool touchEventsBool
    ){
        return handle->api.startUpHID(vendor_id, product_id, keyEventsBool, touchEventsBool);
    }
    /**
     * @function graphiti_shutDownHID
     * @brief Shuts down the HID connection.
     * @param handle Graphiti handle.
     * @param keyEventsBool Disable key events.
     * @param touchEventsBool Disable touch events.
     */
    void graphiti_shutDownHID(GraphitiHandle* handle, 
        bool keyEventsBool, bool touchEventsBool
    ){
        handle->api.shutDownHID(keyEventsBool, touchEventsBool);
    }
    /**
     * @function graphiti_index
     * @brief Computes buffer index for given row and column.
     *
     * @param handle Graphiti handle
     * @param row Pixel row
     * @param col Pixel column
     * @return int Linear buffer index
     */
    int graphiti_index(GraphitiHandle* handle, int row, int col) {
       return handle->api.index(row, col);
    }
    /**
     * @function graphiti_setPin
     * @brief Sets a single tactile pin height in buffer.
     *
     * @param handle Graphiti handle
     * @param row Pin row
     * @param col Pin column
     * @param height Pin height
     */
    void graphiti_setPin(GraphitiHandle* handle, int row, int col, int height){
         handle->api.setPin(row, col, height);
    }
    /**
    * @function graphiti_clearScreen
    * @brief Clears the internal screen buffer.
    *
    * @param handle Graphiti handle
    */
    void graphiti_clearScreen(GraphitiHandle* handle) {
        handle->api.clearScreen();
    }
    /**
    * @function graphiti_sleep
    * @brief Pauses execution for given duration.
    *
    * @param handle Graphiti handle
    * @param time Delay duration in seconds
    */
    void graphiti_sleep(GraphitiHandle* handle, int time){
       handle->api.sleep(time);
    }

    /**
     * @function: graphiti_startResponseThread
     * @brief: Start background response processing thread.
     * @param handle Graphiti handle.
     */
    void graphiti_startResponseThread(GraphitiHandle* handle) {
        handle->api.startResponseThread();
    }
    /**
     * @function graphiti_stopResponseThread
     * @brief Stop the background response processing thread.
     * @param handle Graphiti handle.
     */
    void graphiti_stopResponseThread(GraphitiHandle* handle) {
        
        handle->api.stopResponseThread();
    }
    /**
     * @function graphiti_getNextOutputEvent
     * @brief Pop the next ASCII output event into caller buffer.
     * @param handle Graphiti handle.
     * @param out Output buffer.
     * @param max_len Maximum buffer length.
     */
    void graphiti_getNextOutputEvent(GraphitiHandle* handle, char* out, size_t max_len) {
        if (!out || max_len == 0) return;

        auto event = handle->api.getNextOutputEvent();
        if (event && !event->empty()) {
            std::strncpy(out, event->c_str(), max_len - 1);
            out[max_len - 1] = '\0';  // ensure null-termination
        } else {
            out[0] = '\0';  // write empty string
        }
    }
    /**
     * @function graphiti_getNextDisplayStatusEvent
     * @brief Retrieve next display status event into integer buffer.
     * @param handle Graphiti handle.
     * @param buffer Output integer buffer.
     * @param max_len Maximum number of integers.
     * @return int Number of integers written.
     */
    int graphiti_getNextDisplayStatusEvent(GraphitiHandle* handle, int* buffer, int max_len) {
        if (!buffer || max_len <= 0) return 0;

        auto event = handle->api.getNextDisplayStatusEvent();
        if (!event) return 0;

        int count = std::min<int>(event->size(), max_len);
        memcpy(buffer, event->data(), count * sizeof(int));
        return count;
    }
    /**
     * @function graphiti_getNextKeyEvent
     * @brief Retrieve next key event into provided string buffers.
     * @param handle Graphiti handle.
     * @param out_keys Output string buffers.
     * @param max_string_len Maximum string length per key.
     * @return int Number of keys written.
     */
    int graphiti_getNextKeyEvent(GraphitiHandle* handle, char** out_keys, int max_string_len) {
        if (!handle || !out_keys || max_string_len <= 0)
            return 0;

        constexpr int MAX_KEYS = 14;

        auto event = handle->api.getNextKeyEvent();  // std::optional<std::vector<std::string>>
        if (!event)
            return 0;

        int i = 0;
        for (const std::string& key : *event) {
            if (i >= MAX_KEYS) break;
            std::strncpy(out_keys[i], key.c_str(), max_string_len - 1);
            out_keys[i][max_string_len - 1] = '\0';
            ++i;
        }

        return i;
    }
    /**
     * @function graphiti_getNextGestureEvent
     * @brief Pop the next gesture event into caller buffer.
     * @param handle Graphiti handle.
     * @param out Output buffer.
     * @param max_len Maximum buffer length.
     */
    void graphiti_getNextGestureEvent(GraphitiHandle* handle, char* out, size_t max_len) {
        if (!out || max_len == 0) return;

        auto event = handle->api.getNextOutputEvent();
        if (event && !event->empty()) {
            std::strncpy(out, event->c_str(), max_len - 1);
            out[max_len - 1] = '\0';  // ensure null-termination
        } else {
            out[0] = '\0';  // write empty string
        }
    }
    /**
     * @function graphiti_getNextDrawEvent
     * @brief Fill provided PinInfo_C array with next draw event pins.
     * @param handle Graphiti handle.
     * @param out_pins Output PinInfo_C array.
     * @param max_len Maximum number of pins.
     * @return int Number of pins written.
     */
    int graphiti_getNextDrawEvent(GraphitiHandle* handle, PinInfo_C* out_pins, int max_len) {
        if (!handle || !out_pins || max_len <= 0)
            return 0;

        auto event = handle->api.getNextDrawEvent();  // returns optional<DrawEvent>
        if (!event)
            return 0;

        int count = std::min<int>(event->pins.size(), max_len);
        for (int i = 0; i < count; ++i) {
            out_pins[i].rowID      = event->pins[i].rowID;
            out_pins[i].columnID   = event->pins[i].columnID;
            out_pins[i].height     = event->pins[i].height;
            out_pins[i].blinkRate  = event->pins[i].blinkRate;
        }

        return count;
    }
    /**
     * @function graphiti_sendACK
     * @brief Send ACK to device.
     * @param handle Graphiti handle.
     */
    void graphiti_sendACK(GraphitiHandle* handle) {
        handle->api.sendACK();
    }
    /**
     * @function graphiti_sendNACK
     * @brief Send NACK to device.
     * @param handle Graphiti handle.
     */
    void graphiti_sendNACK(GraphitiHandle* handle) {
        handle->api.sendNACK();
    }
    /**
     * @function graphiti_getSoftwareVersion
     * @brief Request software version from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getSoftwareVersion(GraphitiHandle* handle) {
        handle->api.getSoftwareVersion();
    }
    /**
     * @function graphiti_getHardwareVersion
     * @brief Request hardware version from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getHardwareVersion(GraphitiHandle* handle) {
        handle->api.getHardwareVersion();
    }
    /**
     * @function graphiti_getSerialNumber
     * @brief Request serial number from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getSerialNumber(GraphitiHandle* handle) {
        handle->api.getSerialNumber();
    }
    /**
     * @function graphiti_getBatteryStatus
     * @brief Request battery status from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getBatteryStatus(GraphitiHandle* handle) {
        handle->api.getBatteryStatus();
    }
    /**
     * @function graphiti_getResolutionInformation
     * @brief Request resolution information from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getResolutionInformation(GraphitiHandle* handle) {
        handle->api.getResolutionInformation();
    }
    /**
     * @function graphiti_getDeviceOrientation
     * @brief Request device orientation from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getDeviceOrientation(GraphitiHandle* handle) {
        handle->api.getDeviceOrientation();
    }
    /**
     * @function graphiti_getHeightInformation
     * @brief Request height information from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getHeightInformation(GraphitiHandle* handle) {
        handle->api.getHeightInformation();
    }
    /**
     * @function graphiti_getDeviceName
     * @brief Request device name from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getDeviceName(GraphitiHandle* handle) {
        handle->api.getDeviceName();
    }
    /**
     * @function graphiti_updateDisplay
     * @brief Update display with provided screen data.
     * @param handle Graphiti handle.
     * @param screen_data Screen data buffer.
     * @param length Length of screen data.
     */
    void graphiti_updateDisplay(GraphitiHandle* handle, const uint8_t* screen_data, int length) {
        std::vector<uint8_t> data(screen_data, screen_data + length);
        handle->api.updateDisplay(data);
    }
    /**
     * @function graphiti_setDisplay
     * @brief Set display mode on device.
     * @param handle Graphiti handle.
     */
    void graphiti_setDisplay(GraphitiHandle* handle) {
        handle->api.setDisplay();
    }
    /**
     * @function graphiti_clearDisplay
     * @brief Clear display on device.
     * @param handle Graphiti handle.
     */
    void graphiti_clearDisplay(GraphitiHandle* handle) {
        handle->api.clearDisplay();
    }
    /**
     * @function graphiti_updateSinglePixel
     * @brief Update a single pixel on device.
     * @param handle Graphiti handle.
     * @param row Row index.
     * @param column Column index.
     * @param height Height value.
     * @param blinkRate Blink rate value.
     */
    void graphiti_updateSinglePixel(GraphitiHandle* handle, int row, int column, int height, int blinkRate) {
        handle->api.updateSinglePixel(row, column, height, blinkRate);
    }
    /**
     * @function graphiti_updateSingleRow
     * @brief Update a single row on device.
     * @param handle Graphiti handle.
     * @param row Row index.
     * @param rowData Row data buffer.
     * @param length Length of row data.
     */
    void graphiti_updateSingleRow(GraphitiHandle* handle, int row, const uint8_t* rowData, int length) {
        std::vector<uint8_t> data(rowData, rowData + length);
        handle->api.updateSingleRow(row, data);
    }
    /**
     * @function graphiti_updateSingleColumn
     * @brief Update a single column on device.
     * @param handle Graphiti handle.
     * @param column Column index.
     * @param columnData Column data buffer.
     * @param length Length of column data.
     */
    void graphiti_updateSingleColumn(GraphitiHandle* handle, int column, const uint8_t* columnData, int length) {
        std::vector<uint8_t> data(columnData, columnData + length);
        handle->api.updateSingleColumn(column, data);
    }
    /**
     * @function graphiti_getAllPixelsPositionStatus
     * @brief Request all pixels position status from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getAllPixelsPositionStatus(GraphitiHandle* handle) {
        handle->api.getAllPixelsPositionStatus();
    }
    /**
     * @function graphiti_getSinglePixelPositionStatus
     * @brief Request single pixel position status from device.
     * @param handle Graphiti handle.
     * @param row Row index.
     * @param column Column index.
     */
    void graphiti_getSinglePixelPositionStatus(GraphitiHandle* handle, int row, int column) {
        handle->api.getSinglePixelPositionStatus(row, column);
    }
    /**
     * @function graphiti_getSingleRowPixelPositionStatus
     * @brief Request single row pixel position status from device.
     * @param handle Graphiti handle.
     * @param row Row index.
     */
    void graphiti_getSingleRowPixelPositionStatus(GraphitiHandle* handle, uint8_t row) {
        handle->api.getSingleRowPixelPositionStatus(row);
    }
    /**
     * @function graphiti_getSingleColumnPixelPositionStatus
     * @brief Request single column pixel position status from device.
     * @param handle Graphiti handle.
     * @param column Column index.
     */
    void graphiti_getSingleColumnPixelPositionStatus(GraphitiHandle* handle, uint8_t column) {
        handle->api.getSingleColumnPixelPositionStatus(column);
    }
    /**
     * @function graphiti_showMessage
     * @brief Display a message on the device.
     * @param handle Graphiti handle.
     * @param message Message string.
     * @param selectFlags Selection flags buffer.
     * @param flagLength Length of selection flags.
     * @param blinkRate Blink rate value.
     */
    void graphiti_showMessage(GraphitiHandle* handle, const char* message, const uint8_t* selectFlags, int flagLength, int blinkRate) {
        std::string msg(message);
        std::vector<uint8_t> flags(selectFlags, selectFlags + flagLength);
        handle->api.showMessage(msg, flags, blinkRate);
    }
    /**
     * @function graphiti_setCursor
     * @brief Set cursor on device.
     * @param handle Graphiti handle.
     * @param row Row index.
     * @param column Column index.
     * @param height Height value.
     * @param length Cursor length.
     * @param blinkRate Blink rate value.
     */
    void graphiti_setCursor(GraphitiHandle* handle, int row, int column, int height, int length, int blinkRate) {
        handle->api.setCursor(row, column, height, length, blinkRate);
    }
    /**
     * @function graphiti_sendImageInterruptible
     * @brief Send image (interruptible) to device.
     * @param handle Graphiti handle.
     * @param name Image name.
     * @param filepath Image file path.
     */
    void graphiti_sendImageInterruptible(GraphitiHandle* handle, const char* name, const char* filepath) {
        handle->api.sendImageInterruptible(std::string(name), std::string(filepath));
    }
    /**
     * @function graphiti_sendImageBlocking
     * @brief Send image (blocking) to device.
     * @param handle Graphiti handle.
     * @param name Image name.
     * @param filepath Image file path.
     */
    void graphiti_sendImageBlocking(GraphitiHandle* handle, const char* name, const char* filepath) {
        handle->api.sendImageBlocking(std::string(name), std::string(filepath));
    }
    /**
     * @function graphiti_setKeyEvent
     * @brief Enable or disable key event notifications.
     * @param handle Graphiti handle.
     * @param enabled True to enable, false to disable.
     */
    void graphiti_setKeyEvent(GraphitiHandle* handle, bool enabled) {
        handle->api.setKeyEvent(enabled);
    }
    /**
     * @function graphiti_setTouchEvent
     * @brief Enable or disable touch event notifications.
     * @param handle Graphiti handle.
     * @param enabled True to enable, false to disable.
     */
    void graphiti_setTouchEvent(GraphitiHandle* handle, bool enabled) {
        handle->api.setTouchEvent(enabled);
    }
    /**
     * @function graphiti_getLastTouchPointStatus
     * @brief Request last touch point status from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getLastTouchPointStatus(GraphitiHandle* handle) {
        handle->api.getLastTouchPointStatus();
    }
    /**
     * @function graphiti_vibratorControlCommand
     * @brief Control vibrator motor on device.
     * @param handle Graphiti handle.
     * @param frequencyRange Frequency range.
     * @param dutyCycle Duty cycle.
     * @param duration Duration in ms.
     */
    void graphiti_vibratorControlCommand(GraphitiHandle* handle, int frequencyRange, int dutyCycle, int duration) {
        handle->api.vibratorControlCommand(frequencyRange, dutyCycle, duration);
    }
    /**
     * @function graphiti_getDateAndTime
     * @brief Request date and time from device.
     * @param handle Graphiti handle.
     */
    void graphiti_getDateAndTime(GraphitiHandle* handle) {
        handle->api.getDateAndTime();
    }
    /**
     * @function graphiti_setDateAndTime
     * @brief Set date and time on device.
     * @param handle Graphiti handle.
     * @param day Day value.
     * @param month Month value.
     * @param year Year value.
     * @param hour Hour value.
     * @param minute Minute value.
     * @param second Second value.
     */
    void graphiti_setDateAndTime(GraphitiHandle* handle, int day, int month, int year, int hour, int minute, int second) {
        handle->api.setDateAndTime(day, month, year, hour, minute, second);
    }
}