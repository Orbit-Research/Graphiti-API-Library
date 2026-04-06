/**
 ******************************************************************************
 * @file    Graphiti.java
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   Java binding facade for Graphiti native library.
 *
 * @details
 * Declares the Java-side wrapper and native method signatures used by the
 * JNI bridge. Users should instantiate `Graphiti` and call methods which
 * forward to the native implementation in the `libGraphitiJNI` library.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 ******************************************************************************
  */

package binding.jgraphiti;

import java.util.Set;

/**
 * @brief Java JNI wrapper class for Graphiti device API
 */
public class Graphiti {
        /**
         * @function getLastHIDConnectionInfo
         * @brief Returns a string with the last connected HID VID, PID, and interface
         * @return String info (e.g. "VID=0x0483, PID=0xA366, interface=1")
         */
        public static native String getLastHIDConnectionInfo();
    static { System.loadLibrary("libGraphitiJNI"); }
    private long nativeHandle;

    /**
     * @function getHandle
     * @brief Returns the native Graphiti handle
     * @return long - native handle value
     */
    public long getHandle() {
        return nativeHandle;
    }

    /**
     * @struct PinInfo
     * @brief Holds tactile pin information
     */
    public static class PinInfo {
        /** @brief Row index (1–40) */
        public int rowID;

        /** @brief Column index (1–60) */
        public int columnID;

        /** @brief Pin height (0–4) */
        public int height;

        /** @brief Blink rate (0–50) */
        public int blinkRate;
    }

    /**
     * @struct DrawEvent
     * @brief Holds draw event information
     */
    public static class DrawEvent {
        public int length;

        /** @brief Array of pin data */
        public PinInfo[] pins;
    }

    /**
     * @function Graphiti
     * @brief Constructs a new Graphiti object and initializes native handle
     */
    public Graphiti() { nativeHandle = graphiti_create(); }

    /**
     * @function close
     * @brief Destroys the native Graphiti handle and releases resources
     */
    public void close() { graphiti_destroy(nativeHandle); }

    /**
     * @function graphiti_create
     * @brief Creates a native Graphiti handle
     * @return long  native handle
     */
    public native long graphiti_create();

    /**
     * @function graphiti_destroy
     * @brief Destroys a native Graphiti handle
     * @param handle Native Graphiti handle
     */
    public native void graphiti_destroy(long handle);
    
    /**
     * @function startUpVCP
     * @brief Starts Graphiti communication over VCP
     * @param handle Native Graphiti handle
     * @param port Serial port name
     * @param keyEvents Enable key events
     * @param touchEvents Enable touch events
     * @return boolean - true on success
     */
    public native boolean startUpVCP(long handle, String port, boolean keyEvents, boolean touchEvents);

    /**
     * @function shutDownVCP
     * @brief Shuts down VCP connection
     * @param handle Native Graphiti handle
     * @param key Disable key events
     * @param touch Disable touch events
     */
    public native void shutDownVCP(long handle, boolean key, boolean touch);

    /**
     * @function startUpHID
     * @brief Starts Graphiti communication over HID
     * @param handle Native Graphiti handle
     * @param vendor_id USB vendor ID
     * @param product_id USB product ID
     * @param keyEvents Enable key events
     * @param touchEvents Enable touch events
     * @return boolean - true on success
     */
    /**
     * @function startUpHID
     * @brief Starts Graphiti communication over HID
     * @param handle Native Graphiti handle
     * @param vendor_id USB vendor ID
     * @param product_id USB product ID
     * @param keyEvents Enable key events
     * @param touchEvents Enable touch events
     * @return boolean - true on success
     *
     * Logic:
     *   - For VID=0x0483, PID=0xA366: Only HID interface 1 is supported (enforced in native/JNI)
     *   - For VID=0x1FC9, PID=0x8217: Any interface is allowed
     */
    public native boolean startUpHID(long handle, int vendor_id, int product_id, boolean keyEvents, boolean touchEvents);

    /**
     * @function shutDownHID
     * @brief Shuts down HID connection
     * @param handle Native Graphiti handle
     * @param key Disable key events
     * @param touch Disable touch events
     */
    public native void shutDownHID(long handle, boolean key, boolean touch);

    /**
     * @function index
     * @brief Calculates linear index from row and column
     * @param handle Native Graphiti handle
     * @param row Row index
     * @param col Column index
     * @return int - calculated index
     */
    public native int index(long handle, int row, int col);

    /**
     * @function setPin
     * @brief Sets a single pin height
     * @param handle Native Graphiti handle
     * @param row Row index
     * @param col Column index
     * @param height Pin height
     */
    public native void setPin(long handle, int row, int col, int height);

    /**
     * @function clearScreen
     * @brief Clears the display
     * @param handle Native Graphiti handle
     */
    public native void clearScreen(long handle);

    /**
     * @function sleep
     * @brief Delays execution
     * @param handle Native Graphiti handle
     * @param time Delay duration in seconds
     */    
    public native void sleep(long handle, int time);

    //Event Handlers
    /**
     * @function getNextOutputEvent
     * @brief Retrieves the next output event
     * @param handle Native Graphiti handle
     * @return String - output event
     */
    public native String getNextOutputEvent(long handle);

    /**
     * @function getNextGestureEvent
     * @brief Retrieves the next gesture event
     * @param handle Native Graphiti handle
     * @return String - gesture event
     */
    public native String getNextGestureEvent(long handle);

    /**
     * @function getDisplayStatusEvent
     * @brief Retrieves display status data
     * @param handle Native Graphiti handle
     * @return int[] - display status buffer
     */
    public native int[] getDisplayStatusEvent(long handle);

    /**
     * @function getNextKeyEvent
     * @brief Retrieves the next key event
     * @param handle Native Graphiti handle
     * @return Set<String> - key event set
     */
    public native Set<String> getNextKeyEvent(long handle);

    /**
     * @function getNextDrawEvent
     * @brief Retrieves the next draw event
     * @param handle Native Graphiti handle
     * @return DrawEvent - draw event object
     */
    public native DrawEvent getNextDrawEvent(long handle); 

    /**
     * @function startResponseThread
     * @brief Starts background response reader thread
     * @param handle Native Graphiti handle
     */
    public native void startResponseThread(long handle);

    /**
     * @function stopResponseThread
     * @brief Stops background response reader thread
     * @param handle Native Graphiti handle
     */
    public native void stopResponseThread(long handle);

    /**
     * @function sendACK
     * @brief Sends ACK to device
     * @param handle Native Graphiti handle
     */
    public native void sendACK(long handle);

    /**
     * @function sendNACK
     * @brief Sends NACK to device
     * @param handle Native Graphiti handle
     */
    public native void sendNACK(long handle);

    /**
     * @function getSoftwareVersion
     * @brief Requests the software version from the device.
     * @param handle Native Graphiti handle.
     */
    public native void getSoftwareVersion(long handle);

    /**
     * @function getSoftwareVersion
     * @brief Requests the software version from the device.
     * @param handle Native Graphiti handle.
     */
    public native void getHardwareVersion(long handle);

    /**
     * @function getSerialNumber
     * @brief Requests the device serial number.
     * @param handle Native Graphiti handle.
     */
    public native void getSerialNumber(long handle);

    /**
     * @function getBatteryStatus
     * @brief Requests the current battery status of the device.
     * @param handle Native Graphiti handle.
     */
    public native void getBatteryStatus(long handle);

    /**
     * @function getResolutionInformation
     * @brief Requests the resolution information of the device.
     * @param handle Native Graphiti handle.
     */
    public native void getResolutionInformation(long handle);

    /**
     * @function getDeviceOrientation
     * @brief Requests the current orientation of the device.
     * @param handle Native Graphiti handle.
     */
    public native void getDeviceOrientation(long handle);

    /**
     * @function getHeightInformation
     * @brief Requests the supported height levels of the device.
     * @param handle Native Graphiti handle.
     */
    public native void getHeightInformation(long handle);

    /**
     * @function getDeviceName
     * @brief Requests the device name.
     * @param handle Native Graphiti handle.
     */
    public native void getDeviceName(long handle);

    /**
     * @function updateDisplay
     * @brief Updates the full tactile display using provided screen data.
     * @param handle Native Graphiti handle.
     * @param screen Byte array representing screen data.
     * @return void
     */
    public native void updateDisplay(long handle, byte[] screen);

    /**
     * @function setDisplay
     * @brief Applies the prepared display buffer to the device.
     * @param handle Native Graphiti handle.
     */
    public native void setDisplay(long handle);

    /**
     * @function clearDisplay
     * @brief Clears the device display by lowering all pins.
     * @param handle Native Graphiti handle.
     */    
    public native void clearDisplay(long handle);

    /**
     * @function updateSinglePixel
     * @brief Updates a single tactile pixel on the device.
     * @param handle Native Graphiti handle.
     * @param row Pixel row index.
     * @param column Pixel column index.
     * @param height Pixel height (0–4).
     * @param blinkRate Blink rate of the pixel.
     */
    public native void updateSinglePixel(long handle, int row, int column, int height, int blinkRate);

    /**
     * @function updateSingleRow
     * @brief Updates all pixels in a specific row.
     * @param handle Native Graphiti handle.
     * @param row Row index.
     * @param rowData Byte array representing row data.
     */
    public native void updateSingleRow(long handle, int row, byte[] rowData);

    /**
     * @function updateSingleColumn
     * @brief Updates all pixels in a specific column.
     * @param handle Native Graphiti handle.
     * @param column Column index.
     * @param columnData Byte array representing column data.
     */
    public native void updateSingleColumn(long handle, int column, byte[] columnData);

    /**
     * @function getAllPixelsPositionStatus
     * @brief Requests position status of all pixels.
     * @param handle Native Graphiti handle.
     */
    public native void getAllPixelsPositionStatus(long handle);

    /**
     * @function getSinglePixelPositionStatus
     * @brief Requests position status of a single pixel.
     * @param handle Native Graphiti handle.
     * @param row Pixel row index.
     * @param column Pixel column index.
     */
    public native void getSinglePixelPositionStatus(long handle, int row, int column);

    /**
     * @function getSingleRowPixelPositionStatus
     * @brief Requests position status of a specific row.
     * @param handle Native Graphiti handle.
     * @param row Row index.
     */
    public native void getSingleRowPixelPositionStatus(long handle, int row);

    /**
     * @function getSingleColumnPixelPositionStatus
     * @brief Requests position status of a specific column.
     * @param handle Native Graphiti handle.
     * @param column Column index.
     */
    public native void getSingleColumnPixelPositionStatus(long handle, int column);

    /**
     * @function showMessage
     * @brief Displays a text message on the device.
     * @param handle Native Graphiti handle.
     * @param message Message string.
     * @param selectFlags Selection flags.
     * @param blinkRate Blink rate for the message.
     */
    public native void showMessage(long handle, String message, byte[] selectFlags, int blinkRate);

    /**
     * @function setCursor
     * @brief Sets the cursor position and properties.
     * @param handle Native Graphiti handle.
     * @param row Cursor row.
     * @param column Cursor column.
     * @param height Cursor height.
     * @param length Cursor length.
     * @param blinkRate Cursor blink rate.
     */
    public native void setCursor(long handle, int row, int column, int height, int length, int blinkRate);

    /**
     * @function sendImageInterruptible
     * @brief Sends an image to the device with interrupt support.
     * @param handle Native Graphiti handle.
     * @param name Image name.
     * @param filepath Image file path.
     */
    public native void sendImageInterruptible(long handle, String name, String filepath);

    /**
     * @function sendImageBlocking
     * @brief Sends an image to the device in blocking mode.
     * @param handle Native Graphiti handle.
     * @param name Image name.
     * @param filepath Image file path.
     * @return void
     */
    public native void sendImageBlocking(long handle, String name, String filepath);

    /**
     * @function setKeyEvent
     * @brief Enables or disables key events.
     * @param handle Native Graphiti handle.
     * @param enabled Enable or disable flag.
     */
    public native void setKeyEvent(long handle, boolean enabled);

    /**
     * @function setTouchEvent
     * @brief Enables or disables touch events.
     * @param handle Native Graphiti handle.
     * @param enabled Enable or disable flag.
     */
    public native void setTouchEvent(long handle, boolean enabled);

    /**
     * @function getLastTouchPointStatus
     * @brief Requests the last detected touch point status.
     * @param handle Native Graphiti handle.
     */
    public native void getLastTouchPointStatus(long handle);

    /**
     * @function vibratorControlCommand
     * @brief Controls the vibration motor.
     * @param handle Native Graphiti handle.
     * @param frequencyRange Frequency range.
     * @param dutyCycle Duty cycle.
     * @param duration Duration in milliseconds.
     */
    public native void vibratorControlCommand(long handle, int frequencyRange, int dutyCycle, int duration);

    /**
     * @function getDateAndTime
     * @brief Requests the current date and time from the device.
     * @param handle Native Graphiti handle.
     */
    public native void getDateAndTime(long handle);

    /**
     * @function setDateAndTime
     * @brief Sets the device date and time.
     * @param handle Native Graphiti handle.
     * @param day Day value.
     * @param month Month value.
     * @param year Year value.
     * @param hour Hour value.
     * @param minute Minute value.
     * @param second Second value.
     */
    public native void setDateAndTime(long handle, int day, int month, int year, int hour, int minute, int second);
}

