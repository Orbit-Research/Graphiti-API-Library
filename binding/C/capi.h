/**
  ******************************************************************************
  * @file    lib/include/CWrapper/graphiti_capi.h
  * @company Orbit Research
  * @version v1.0
  * @date    08-12-25
  * @brief   C API wrapper for the Graphiti native library.
  *
  * @details
  * Exposes a C-compatible ABI that wraps the Graphiti C++ classes, enabling
  * usage from languages like Python and Java (via JNI). Ownership rules for
  * returned pointers are documented in function comments.
  *
  ******************************************************************************
  * @attention
  * Copyright (c) 2025 Orbit Research LLC
  * Distributed under GNU General Public License V3.0
  ******************************************************************************
  */


#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  #ifdef BUILDING_GRAPHITI_DLL
    #define GRAPHITI_API __declspec(dllexport)
  #else
    #define GRAPHITI_API __declspec(dllimport)
  #endif
#else
  #define GRAPHITI_API
#endif

/**
 * @brief Opaque handle for Graphiti instance
 */
typedef struct GraphitiHandle GraphitiHandle;

/**
 * @function graphiti_create
 * @brief Creates a new Graphiti instance
 * @return Pointer to GraphitiHandle
 */
GRAPHITI_API GraphitiHandle* graphiti_create();

/**
 * @function graphiti_createWithConnection
 * @brief Creates a Graphiti instance with an existing connection
 * @param connection Pointer to native GraphitiConnection
 * @return Pointer to GraphitiHandle
 */
GRAPHITI_API GraphitiHandle* graphiti_createWithConnection(void* connection);

/**
 * @function graphiti_destroy
 * @brief Destroys a Graphiti instance and releases resources
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_destroy(GraphitiHandle* handle);

/**
 * @function graphiti_setConnection
 * @brief Sets the connection object for Graphiti
 * @param handle Graphiti handle
 * @param connection Pointer to native GraphitiConnection
 * @return void
 */
GRAPHITI_API void graphiti_setConnection(GraphitiHandle* handle, void* connection);

/**
 * @function graphiti_startUpVCP
 * @brief Starts VCP connection and performs startup sequence
 * @param handle Graphiti handle
 * @param portName Serial port name
 * @param keyEventsBool Enable key events
 * @param touchEventsBool Enable touch events
 * @return true on success, false on failure
 */
GRAPHITI_API bool graphiti_startUpVCP(
    GraphitiHandle* handle,
    const char* portName,
    bool keyEventsBool,
    bool touchEventsBool
);

/**
 * @function graphiti_shutDownVCP
 * @brief Shuts down VCP connection
 * @param handle Graphiti handle
 * @param keyEventsBool Disable key events
 * @param touchEventsBool Disable touch events
 * @return void
 */
GRAPHITI_API void graphiti_shutDownVCP(
    GraphitiHandle* handle,
    bool keyEventsBool,
    bool touchEventsBool
);

/**
 * @function graphiti_startUpHID
 * @brief Starts HID connection and performs startup sequence
 * @param handle Graphiti handle
 * @param vendor_id USB vendor ID
 * @param product_id USB product ID
 * @param keyEventsBool Enable key events
 * @param touchEventsBool Enable touch events
 * @return true on success, false on failure
 */
GRAPHITI_API bool graphiti_startUpHID(
    GraphitiHandle* handle,
    uint16_t vendor_id,
    uint16_t product_id,
    bool keyEventsBool,
    bool touchEventsBool
);

/**
 * @function graphiti_shutDownHID
 * @brief Shuts down HID connection
 * @param handle Graphiti handle
 * @param keyEventsBool Disable key events
 * @param touchEventsBool Disable touch events
 * @return void
 */
GRAPHITI_API void graphiti_shutDownHID(
    GraphitiHandle* handle,
    bool keyEventsBool,
    bool touchEventsBool
);

/**
 * @function graphiti_index
 * @brief @return linear index for a screen position
 * @param handle Graphiti handle
 * @param row Screen row
 * @param col Screen column
 * @return Linear index
 */
GRAPHITI_API int graphiti_index(GraphitiHandle* handle, int row, int col);

/**
 * @function graphiti_setPin
 * @brief Sets a single pin height
 * @param handle Graphiti handle
 * @param row Screen row
 * @param col Screen column
 * @param height Pin height
 * @return void
 */
GRAPHITI_API void graphiti_setPin(GraphitiHandle* handle, int row, int col, int height);

/**
 * @function graphiti_clearScreen
 * @brief Clears the screen buffer
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_clearScreen(GraphitiHandle* handle);

/**
 * @function graphiti_sleep
 * @brief Sleeps execution for given time
 * @param handle Graphiti handle
 * @param time Time in seconds
 * @return void
 */
GRAPHITI_API void graphiti_sleep(GraphitiHandle* handle, int time);

/**
 * @function graphiti_startResponseThread
 * @brief Starts background response thread
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_startResponseThread(GraphitiHandle* handle);

/**
 * @function graphiti_stopResponseThread
 * @brief Stops background response thread
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_stopResponseThread(GraphitiHandle* handle);

/**
 * @function graphiti_getNextOutputEvent
 * @brief Retrieves next output event as string
 * @param handle Graphiti handle
 * @param out Output buffer
 * @param max_len Maximum buffer length
 * @return void
 */
GRAPHITI_API void graphiti_getNextOutputEvent(
    GraphitiHandle* handle,
    char* out,
    size_t max_len
);

/**
 * @function graphiti_getNextDisplayStatusEvent
 * @brief Retrieves next display status event
 * @param handle Graphiti handle
 * @param buffer Output buffer
 * @param max_len Maximum number of entries
 * @return Number of elements written
 */
GRAPHITI_API int graphiti_getNextDisplayStatusEvent(
    GraphitiHandle* handle,
    int* buffer,
    int max_len
);

/**
 * @function graphiti_getNextKeyEvent
 * @brief Retrieves next key event
 * @param handle Graphiti handle
 * @param buffer Output buffer
 * @param max_len Maximum length
 * @return Number of events read
 */
GRAPHITI_API int graphiti_getNextKeyEvent(
    GraphitiHandle* handle,
    char** buffer,
    int max_len
);

/**
 * @function graphiti_getNextGestureEvent
 * @brief Retrieves next gesture event
 * @param handle Graphiti handle
 * @param out Output buffer
 * @param max_len Maximum buffer length
 * @return void
 */
GRAPHITI_API void graphiti_getNextGestureEvent(
    GraphitiHandle* handle,
    char* out,
    size_t max_len
);

/**
 * @struct PinInfo_C
 * @brief C representation of a draw pin
 */
typedef struct {
    int rowID;
    int columnID;
    int height;
    int blinkRate;
} PinInfo_C;

/**
 * @function graphiti_getNextDrawEvent
 * @brief Retrieves next draw event
 * @param handle Graphiti handle
 * @param out_pins Output pin array
 * @param max_len Maximum pins
 * @return Number of pins written
 */
GRAPHITI_API int graphiti_getNextDrawEvent(
    GraphitiHandle* handle,
    PinInfo_C* out_pins,
    int max_len
);

/**
 * @function graphiti_sendACK
 * @brief Sends ACK to device
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_sendACK(GraphitiHandle* handle);

/**
 * @function graphiti_sendNACK
 * @brief Sends NACK to device
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_sendNACK(GraphitiHandle* handle);

/**
 * @function graphiti_getSoftwareVersion
 * @brief Requests the software version from the device
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getSoftwareVersion(GraphitiHandle* handle);

/**
 * @function graphiti_getHardwareVersion
 * @brief Requests the hardware version from the device
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getHardwareVersion(GraphitiHandle* handle);

/**
 * @function graphiti_getSerialNumber
 * @brief Requests the device serial number
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getSerialNumber(GraphitiHandle* handle);

/**
 * @function graphiti_getBatteryStatus
 * @brief Requests the battery status from the device
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getBatteryStatus(GraphitiHandle* handle);

/**
 * @function graphiti_getResolutionInformation
 * @brief Requests the screen resolution information
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getResolutionInformation(GraphitiHandle* handle);

/**
 * @function graphiti_getDeviceOrientation
 * @brief Requests the current device orientation
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getDeviceOrientation(GraphitiHandle* handle);

/**
 * @function graphiti_getHeightInformation
 * @brief Requests pin height capability information
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getHeightInformation(GraphitiHandle* handle);

/**
 * @function graphiti_getDeviceName
 * @brief Requests the device name
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getDeviceName(GraphitiHandle* handle);

/**
 * @function graphiti_updateDisplay
 * @brief Updates the entire display buffer
 * @param handle Graphiti handle
 * @param screen_data Pointer to screen data buffer
 * @param length Length of the buffer
 * @return void
 */
GRAPHITI_API void graphiti_updateDisplay(
    GraphitiHandle* handle,
    const uint8_t* screen_data,
    int length
);

/**
 * @function graphiti_setDisplay
 * @brief Commits the buffered display data to the device
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_setDisplay(GraphitiHandle* handle);

/**
 * @function graphiti_clearDisplay
 * @brief Clears the device display
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_clearDisplay(GraphitiHandle* handle);

/**
 * @function graphiti_updateSinglePixel
 * @brief Updates a single pixel on the display
 * @param handle Graphiti handle
 * @param row Pixel row index
 * @param column Pixel column index
 * @param height Pixel height
 * @param blinkRate Pixel blink rate
 * @return void
 */
GRAPHITI_API void graphiti_updateSinglePixel(
    GraphitiHandle* handle,
    int row,
    int column,
    int height,
    int blinkRate
);

/**
 * @function graphiti_updateSingleRow
 * @brief Updates a single row on the display
 * @param handle Graphiti handle
 * @param row Row index
 * @param rowData Row data buffer
 * @param length Length of the buffer
 * @return void
 */
GRAPHITI_API void graphiti_updateSingleRow(
    GraphitiHandle* handle,
    int row,
    const uint8_t* rowData,
    int length
);

/**
 * @function graphiti_updateSingleColumn
 * @brief Updates a single column on the display
 * @param handle Graphiti handle
 * @param column Column index
 * @param columnData Column data buffer
 * @param length Length of the buffer
 * @return void
 */
GRAPHITI_API void graphiti_updateSingleColumn(
    GraphitiHandle* handle,
    int column,
    const uint8_t* columnData,
    int length
);

/**
 * @function graphiti_getAllPixelsPositionStatus
 * @brief Requests position status for all pixels
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getAllPixelsPositionStatus(GraphitiHandle* handle);

/**
 * @function graphiti_getSinglePixelPositionStatus
 * @brief Requests position status for a single pixel
 * @param handle Graphiti handle
 * @param row Pixel row index
 * @param column Pixel column index
 * @return void
 */
GRAPHITI_API void graphiti_getSinglePixelPositionStatus(
    GraphitiHandle* handle,
    int row,
    int column
);

/**
 * @function graphiti_getSingleRowPixelPositionStatus
 * @brief Requests position status for a single row
 * @param handle Graphiti handle
 * @param row Row index
 * @return void
 */
GRAPHITI_API void graphiti_getSingleRowPixelPositionStatus(
    GraphitiHandle* handle,
    uint8_t row
);

/**
 * @function graphiti_getSingleColumnPixelPositionStatus
 * @brief Requests position status for a single column
 * @param handle Graphiti handle
 * @param column Column index
 * @return void
 */
GRAPHITI_API void graphiti_getSingleColumnPixelPositionStatus(
    GraphitiHandle* handle,
    uint8_t column
);

/**
 * @function graphiti_showMessage
 * @brief Displays a message on the device
 * @param handle Graphiti handle
 * @param message Message string
 * @param selectFlags Selection flags
 * @param flagLength Length of selection flags
 * @param blinkRate Blink rate
 * @return void
 */
GRAPHITI_API void graphiti_showMessage(
    GraphitiHandle* handle,
    const char* message,
    const uint8_t* selectFlags,
    int flagLength,
    int blinkRate
);

/**
 * @function graphiti_setCursor
 * @brief Sets cursor position and display @param
 * @param handle Graphiti handle
 * @param row Cursor row
 * @param column Cursor column
 * @param height Cursor height
 * @param length Cursor length
 * @param blinkRate Cursor blink rate
 * @return void
 */
GRAPHITI_API void graphiti_setCursor(
    GraphitiHandle* handle,
    int row,
    int column,
    int height,
    int length,
    int blinkRate
);

/**
 * @function graphiti_sendImageInterruptible
 * @brief Sends an image to the device (interruptible mode)
 * @param handle Graphiti handle
 * @param name Image identifier
 * @param filepath Path to image file
 * @return void
 */
GRAPHITI_API void graphiti_sendImageInterruptible(
    GraphitiHandle* handle,
    const char* name,
    const char* filepath
);

/**
 * @function graphiti_sendImageBlocking
 * @brief Sends an image to the device (blocking mode)
 * @param handle Graphiti handle
 * @param name Image identifier
 * @param filepath Path to image file
 * @return void
 */
GRAPHITI_API void graphiti_sendImageBlocking(
    GraphitiHandle* handle,
    const char* name,
    const char* filepath
);

/**
 * @function graphiti_setKeyEvent
 * @brief Enables or disables key events
 * @param handle Graphiti handle
 * @param enabled Enable flag
 * @return void
 */
GRAPHITI_API void graphiti_setKeyEvent(GraphitiHandle* handle, bool enabled);

/**
 * @function graphiti_setTouchEvent
 * @brief Enables or disables touch events
 * @param handle Graphiti handle
 * @param enabled Enable flag
 * @return void
 */
GRAPHITI_API void graphiti_setTouchEvent(GraphitiHandle* handle, bool enabled);

/**
 * @function graphiti_getLastTouchPointStatus
 * @brief Requests last touch point status
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getLastTouchPointStatus(GraphitiHandle* handle);

/**
 * @function graphiti_vibratorControlCommand
 * @brief Controls the device vibrator
 * @param handle Graphiti handle
 * @param frequencyRange Frequency range
 * @param dutyCycle Duty cycle
 * @param duration Duration in milliseconds
 * @return void
 */
GRAPHITI_API void graphiti_vibratorControlCommand(
    GraphitiHandle* handle,
    int frequencyRange,
    int dutyCycle,
    int duration
);

/**
 * @function graphiti_getDateAndTime
 * @brief Requests the current device date and time
 * @param handle Graphiti handle
 * @return void
 */
GRAPHITI_API void graphiti_getDateAndTime(GraphitiHandle* handle);

/**
 * @function graphiti_setDateAndTime
 * @brief Sets the device date and time
 * @param handle Graphiti handle
 * @param day Day
 * @param month Month
 * @param year Year
 * @param hour Hour
 * @param minute Minute
 * @param second Second
 * @return void
 */
GRAPHITI_API void graphiti_setDateAndTime(
    GraphitiHandle* handle,
    int day,
    int month,
    int year,
    int hour,
    int minute,
    int second
);
#ifdef __cplusplus
}
#endif
