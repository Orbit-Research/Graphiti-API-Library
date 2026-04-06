"""
graphiti.py
Company: Orbit Research 
Date: 08-12-2025
@brief: High-level Python wrapper for the Graphiti C API.

Details:
Provides pythonic classes and helpers wrapping the Graphiti C ABI. Intended for
Python 3.8+ and requires the compiled Graphiti shared library accessible at runtime.

License: Distributed under GNU General Public License V3.0
Copyright (c) 2025 Orbit Research
"""

from ctypes import (
    CDLL, c_void_p, c_bool, c_int, c_char_p,
    POINTER, Structure, c_uint8, c_size_t, c_uint16,
    byref, create_string_buffer, cast, 
)
from typing import Optional, List, Dict, Union
import os

class DisplayStatusEvent_C(Structure):
    _fields_ = [
        ("data", POINTER(c_uint8)),
        ("length", c_int),
        ("has_value", c_bool)
    ]

class KeyEvent_C(Structure):
    _fields_ = [
        ("keys", POINTER(c_char_p)),
        ("count", c_int),
        ("has_value", c_bool)
    ]

class PinInfo_C(Structure):
    _fields_ = [
        ("rowID", c_int),
        ("columnID", c_int),
        ("height", c_int),
        ("blinkRate", c_int)
    ]

class DrawEvent_C(Structure):
    _fields_ = [
        ("pins", POINTER(PinInfo_C)),
        ("length", c_int),
        ("has_value", c_bool)
    ]

class Graphiti:
    """High-level Python wrapper around the Graphiti C API.

    This class loads the compiled Graphiti C library via ctypes and exposes
    convenient methods for connecting, sending commands, and reading events.
    """
    # @function __init__
    # @brief Load the Graphiti DLL and prepare ctypes function signatures.
    # @param self The Graphiti instance.
    # @return None. Raises RuntimeError if the DLL cannot be loaded.
    def __init__(self):
        """Load the Graphiti DLL and prepare ctypes function signatures.

        Raises RuntimeError if the DLL cannot be loaded.
        """
        dll_dir = os.path.dirname(os.path.abspath(__file__))
        #print("dll_dir: " + dll_dir)
        os.add_dll_directory(dll_dir)
        dll_path = os.path.join(dll_dir, "libGraphiti_C.dll")
        if not os.path.exists(dll_path):
            dll_path = os.path.join(dll_dir, "Graphiti_C.dll")
        try:
            self._lib = CDLL(dll_path,winmode=0)
        except Exception as e:
            raise RuntimeError(f"Failed to load DLL: {e}")
        self._setup_types()
        self._handle = None
        
    # @function _setup_types
    # @brief Set up ctypes argument and return types for all Graphiti C API functions.
    # @param self The Graphiti instance.
    # @return None
    def _setup_types(self):
        # Handle Management
        self._lib.graphiti_create.restype = c_void_p
        self._lib.graphiti_createWithConnection.argtypes = [c_void_p]
        self._lib.graphiti_createWithConnection.restype = c_void_p
        self._lib.graphiti_destroy.argtypes = [c_void_p]
        self._lib.graphiti_destroy.restype = None
        self._lib.graphiti_setConnection.argtypes = [c_void_p, c_void_p]
        self._lib.graphiti_setConnection.restype = None

        # VCP calls
        self._lib.graphiti_startUpVCP.argtypes = [c_void_p, c_char_p, c_bool, c_bool]
        self._lib.graphiti_startUpVCP.restype = c_bool
        self._lib.graphiti_shutDownVCP.argtypes = [c_void_p, c_bool, c_bool]
        self._lib.graphiti_shutDownVCP.restype = None

        # HID calls
        self._lib.graphiti_startUpHID.argtypes = [c_void_p, c_uint16, c_uint16, c_bool, c_bool]
        self._lib.graphiti_startUpHID.restype = c_bool
        self._lib.graphiti_shutDownHID.argtypes = [c_void_p, c_bool, c_bool]
        self._lib.graphiti_shutDownHID.restype = None

        # Extension
        self._lib.graphiti_index.argtypes = [c_void_p, c_int, c_int]
        self._lib.graphiti_index.restype = c_int
        self._lib.graphiti_setPin.argtypes = [c_void_p, c_int, c_int, c_int]
        self._lib.graphiti_setPin.restype = None
        self._lib.graphiti_clearScreen.argtypes = [c_void_p]
        self._lib.graphiti_clearScreen.restype = None
        self._lib.graphiti_sleep.argtypes = [c_void_p, c_int]
        self._lib.graphiti_setPin.restype = None

        # Response Thread
        self._lib.graphiti_startResponseThread.argtypes = [c_void_p]
        self._lib.graphiti_startResponseThread.restype = None
        self._lib.graphiti_stopResponseThread.argtypes = [c_void_p]
        self._lib.graphiti_stopResponseThread.restype = None

        # Event handling
        self._lib.graphiti_getNextOutputEvent.argtypes = [c_void_p, c_char_p, c_size_t]
        self._lib.graphiti_getNextOutputEvent.restype = None
        self._lib.graphiti_getNextDisplayStatusEvent.argtypes = [c_void_p]
        self._lib.graphiti_getNextDisplayStatusEvent.restype = DisplayStatusEvent_C
        self._lib.graphiti_getNextKeyEvent.argtypes = [c_void_p]
        self._lib.graphiti_getNextKeyEvent.restype = KeyEvent_C
        self._lib.graphiti_getNextGestureEvent.argtypes = [c_void_p, c_char_p, c_size_t]
        self._lib.graphiti_getNextGestureEvent.restype = None
        self._lib.graphiti_getNextDrawEvent.argtypes = [c_void_p]
        self._lib.graphiti_getNextDrawEvent.restype = DrawEvent_C

        # API calls
        self._lib.graphiti_sendACK.argtypes = [c_void_p]
        self._lib.graphiti_sendACK.restype = None
        self._lib.graphiti_sendNACK.argtypes = [c_void_p]
        self._lib.graphiti_sendNACK.restype = None

        self._lib.graphiti_getSoftwareVersion.argtypes = [c_void_p]
        self._lib.graphiti_getSoftwareVersion.restype = None
        self._lib.graphiti_getHardwareVersion.argtypes = [c_void_p]
        self._lib.graphiti_getHardwareVersion.restype = None
        self._lib.graphiti_getSerialNumber.argtypes = [c_void_p]
        self._lib.graphiti_getSerialNumber.restype = None
        self._lib.graphiti_getBatteryStatus.argtypes = [c_void_p]
        self._lib.graphiti_getBatteryStatus.restype = None
        self._lib.graphiti_getResolutionInformation.argtypes = [c_void_p]
        self._lib.graphiti_getResolutionInformation.restype = None
        self._lib.graphiti_getDeviceOrientation.argtypes = [c_void_p]
        self._lib.graphiti_getDeviceOrientation.restype = None
        self._lib.graphiti_getHeightInformation.argtypes = [c_void_p]
        self._lib.graphiti_getHeightInformation.restype = None
        self._lib.graphiti_getDeviceName.argtypes = [c_void_p]
        self._lib.graphiti_getDeviceName.restype = None

        # Display commands
        self._lib.graphiti_updateDisplay.argtypes = [c_void_p, POINTER(c_uint8), c_int]
        self._lib.graphiti_updateDisplay.restype = None
        self._lib.graphiti_setDisplay.argtypes = [c_void_p]
        self._lib.graphiti_setDisplay.restype = None
        self._lib.graphiti_clearDisplay.argtypes = [c_void_p]
        self._lib.graphiti_clearDisplay.restype = None
        self._lib.graphiti_updateSinglePixel.argtypes = [c_void_p, c_int, c_int, c_int, c_int]
        self._lib.graphiti_updateSinglePixel.restype = None
        self._lib.graphiti_updateSingleRow.argtypes = [c_void_p, c_int, POINTER(c_uint8), c_int]
        self._lib.graphiti_updateSingleRow.restype = None
        self._lib.graphiti_updateSingleColumn.argtypes = [c_void_p, c_int, POINTER(c_uint8), c_int]
        self._lib.graphiti_updateSingleColumn.restype = None

        # Status calls
        self._lib.graphiti_getAllPixelsPositionStatus.argtypes = [c_void_p]
        self._lib.graphiti_getAllPixelsPositionStatus.restype = None
        self._lib.graphiti_getSinglePixelPositionStatus.argtypes = [c_void_p, c_int, c_int]
        self._lib.graphiti_getSinglePixelPositionStatus.restype = None
        self._lib.graphiti_getSingleRowPixelPositionStatus.argtypes = [c_void_p, c_int]
        self._lib.graphiti_getSingleRowPixelPositionStatus.restype = None
        self._lib.graphiti_getSingleColumnPixelPositionStatus.argtypes = [c_void_p, c_int]
        self._lib.graphiti_getSingleColumnPixelPositionStatus.restype = None

        self._lib.graphiti_showMessage.argtypes = [c_void_p, c_char_p, POINTER(c_uint8), c_int, c_int]
        self._lib.graphiti_showMessage.restype = None
        self._lib.graphiti_setCursor.argtypes = [c_void_p, c_int, c_int, c_int, c_int, c_int]
        self._lib.graphiti_setCursor.restype = None
        self._lib.graphiti_sendImageInterruptible.argtypes = [c_void_p, c_char_p, c_char_p]
        self._lib.graphiti_sendImageInterruptible.restype = None
        self._lib.graphiti_sendImageBlocking.argtypes = [c_void_p, c_char_p, c_char_p]
        self._lib.graphiti_sendImageBlocking.restype = None

        self._lib.graphiti_setKeyEvent.argtypes = [c_void_p, c_bool]
        self._lib.graphiti_setKeyEvent.restype = None
        self._lib.graphiti_setTouchEvent.argtypes = [c_void_p, c_bool]
        self._lib.graphiti_setTouchEvent.restype = None
        self._lib.graphiti_getLastTouchPointStatus.argtypes = [c_void_p]
        self._lib.graphiti_getLastTouchPointStatus.restype = None
        self._lib.graphiti_vibratorControlCommand.argtypes = [c_void_p, c_int, c_int, c_int]
        self._lib.graphiti_vibratorControlCommand.restype = None

        self._lib.graphiti_getDateAndTime.argtypes = [c_void_p]
        self._lib.graphiti_getDateAndTime.restype = None
        self._lib.graphiti_setDateAndTime.argtypes = [c_void_p, c_int, c_int, c_int, c_int, c_int, c_int]
        self._lib.graphiti_setDateAndTime.restype = None

    # @function graphiti_create
    # @brief Create a native Graphiti handle and store it on this instance.
    # @param self The Graphiti instance.
    # @return None
    def graphiti_create(self):
        self._handle = self._lib.graphiti_create()

    # @function __enter__
    # @brief Context manager enter: return self.
    # @param self The Graphiti instance.
    # @return self
    def __enter__(self):
        return self

    # @function __exit__
    # @brief Context manager exit: destroy native handle if present.
    # @param self The Graphiti instance.
    # @param exc_type Exception type.
    # @param exc_val Exception value.
    # @param exc_tb Exception traceback.
    # @return None
    def __exit__(self, exc_type, exc_val, exc_tb):
        self._lib.graphiti_destroy(self._handle)

    # VCP control
    # @function start_vcp
    # @brief Start a VCP (serial) connection to the device.
    # @param self The Graphiti instance.
    # @param port Serial port string.
    # @param key_events Enable key events (default True).
    # @param touch_events Enable touch events (default True).
    # @return True on success, False on failure.
    def start_vcp(self, port: str, key_events: bool = True, touch_events: bool = True) -> bool:
        return self._lib.graphiti_startUpVCP(
            self._handle,
            port.encode('ascii'),
            c_bool(key_events),
            c_bool(touch_events)
        )

    # @function shutdown_vcp
    # @brief Shutdown a previously opened VCP connection.
    # @param self The Graphiti instance.
    # @param key_events Enable key events (default True).
    # @param touch_events Enable touch events (default True).
    # @return None
    def shutdown_vcp(self, key_events: bool = True, touch_events: bool = True):
        self._lib.graphiti_shutDownVCP(
            self._handle,
            c_bool(key_events),
            c_bool(touch_events)
        )

    # HID control
    # @function start_hid
    # @brief Start a HID connection to the device using vendor/product IDs.
    # @param self The Graphiti instance.
    # @param vendor_id USB vendor ID.
    # @param product_id USB product ID.
    # @param key_events Enable key events (default True).
    # @param touch_events Enable touch events (default True).
    # @return True on success, False on failure.
    def start_hid(self, vendor_id: int, product_id: int, key_events: bool = True, touch_events: bool = True) -> bool:
        return self._lib.graphiti_startUpHID(
            self._handle,
            c_uint16(vendor_id),
            c_uint16(product_id),
            c_bool(key_events),
            c_bool(touch_events)
        )

    # @function shutdown_hid
    # @brief Shutdown a previously opened HID connection.
    # @param self The Graphiti instance.
    # @param key_events Enable key events (default True).
    # @param touch_events Enable touch events (default True).
    # @return None
    def shutdown_hid(self, key_events: bool = True, touch_events: bool = True):
        self._lib.graphiti_shutDownHID(self._handle, c_bool(key_events), c_bool(touch_events))

    # Extension screen management
    # @function get_index
    # @brief Return a library-specific index for the given row/column.
    # @param self The Graphiti instance.
    # @param row Row index.
    # @param col Column index.
    # @return Integer index.
    def get_index(self, row: int, col: int) -> int:
        return self._lib.graphiti_index(self._handle, row, col)

    # @function set_pin
    # @brief Set a single pin to the requested height (immediate command).
    # @param self The Graphiti instance.
    # @param row Row index.
    # @param col Column index.
    # @param height Pin height.
    # @return None
    def set_pin(self, row: int, col: int, height: int):
        self._lib.graphiti_setPin(self._handle, row, col, height)

    # @function clear_screen
    # @brief Clear the entire tactile display.
    # @param self The Graphiti instance.
    # @return None
    def clear_screen(self):
        self._lib.graphiti_clearScreen(self._handle)

    # @function sleep
    # @brief Sleep the device (or pause command processing) for given seconds.
    # @param self The Graphiti instance.
    # @param seconds Number of seconds to sleep.
    # @return None
    def sleep(self, seconds: int):
        self._lib.graphiti_sleep(self._handle, seconds)

    # @function start_response_thread
    # @brief Start an internal thread that listens for device responses/events.
    # @param self The Graphiti instance.
    # @return None
    def start_response_thread(self):
        self._lib.graphiti_startResponseThread(self._handle)

    # @function stop_response_thread
    # @brief Stop the internal response/event thread.
    # @param self The Graphiti instance.
    # @return None
    def stop_response_thread(self):
        self._lib.graphiti_stopResponseThread(self._handle)

    # Event handling
    # @function get_next_output_event
    # @brief Fetch the next ASCII output event from the device, or None if none.
    # @param self The Graphiti instance.
    # @return Output string or None.
    def get_next_output_event(self) -> Optional[str]:
        buf = create_string_buffer(512)
        self._lib.graphiti_getNextOutputEvent(self._handle, buf, len(buf))
        output = buf.value.decode('utf-8')
        return output if output else None

    # @function get_next_display_status_event
    # @brief Get the next display-status event as a list of ints, or None.
    # @param self The Graphiti instance.
    # @return List of ints or None.
    def get_next_display_status_event(self) -> list[int] | None:
        MAX_LEN = 2400
        buffer = (c_int * MAX_LEN)()
        self._lib.graphiti_getNextDisplayStatusEvent.argtypes = [c_void_p, POINTER(c_int), c_int]
        self._lib.graphiti_getNextDisplayStatusEvent.restype = c_int
        count = self._lib.graphiti_getNextDisplayStatusEvent(self._handle, buffer, MAX_LEN)
        if count <= 0:
            return None
        return list(buffer[:count])

    # @function get_next_key_event
    # @brief Fetch the next key event as a list of strings, or None.
    # @param self The Graphiti instance.
    # @return List of key strings or None.
    def get_next_key_event(self) -> list[str] | None:
        MAX_KEYS = 14
        MAX_STRING_LEN = 64
        string_buffers = [create_string_buffer(MAX_STRING_LEN) for _ in range(MAX_KEYS)]
        key_array_type = c_char_p * MAX_KEYS
        key_array = key_array_type(*(cast(buf, c_char_p) for buf in string_buffers))
        self._lib.graphiti_getNextKeyEvent.argtypes = [c_void_p, POINTER(c_char_p), c_int]
        self._lib.graphiti_getNextKeyEvent.restype = c_int
        count = self._lib.graphiti_getNextKeyEvent(self._handle, key_array, MAX_STRING_LEN)
        if count <= 0:
            return None
        return [buf.value.decode("utf-8") for buf in string_buffers[:count]]

    # @function get_next_gesture_event
    # @brief Fetch the next gesture event string from the device, or None.
    # @param self The Graphiti instance.
    # @return Gesture event string or None.
    def get_next_gesture_event(self) -> Optional[str]:
        buf = create_string_buffer(512)
        self._lib.graphiti_getNextGestureEvent(self._handle, buf, len(buf))
        output = buf.value.decode('utf-8')
        return output if output else None

    # @function get_next_draw_event
    # @brief Fetch the next draw event and return a list of PinInfo_C objects.
    # @param self The Graphiti instance.
    # @return List of PinInfo_C or None.
    def get_next_draw_event(self) -> list[PinInfo_C] | None:
        MAX_PINS = 2400
        pins = (PinInfo_C * MAX_PINS)()
        self._lib.graphiti_getNextDrawEvent.argtypes = [c_void_p, POINTER(PinInfo_C), c_int]
        self._lib.graphiti_getNextDrawEvent.restype = c_int
        count = self._lib.graphiti_getNextDrawEvent(self._handle, pins, MAX_PINS)
        if count <= 0:
            return None
        return list(pins[:count])

    # Device control
    # @function send_ack
    # @brief Send an ACK to the device acknowledging the last command.
    # @param self The Graphiti instance.
    # @return None
    def send_ack(self):
        self._lib.graphiti_sendACK(self._handle)

    # @function send_nack
    # @brief Send a NACK to the device to indicate failure or rejection.
    # @param self The Graphiti instance.
    # @return None
    def send_nack(self):
        self._lib.graphiti_sendNACK(self._handle)

    # @function get_software_version
    # @brief Request the device software version (no direct return).
    # @param self The Graphiti instance.
    # @return None
    def get_software_version(self):
        self._lib.graphiti_getSoftwareVersion(self._handle)

    # @function get_hardware_version
    # @brief Request the device hardware version (no direct return).
    # @param self The Graphiti instance.
    # @return None
    def get_hardware_version(self):
        self._lib.graphiti_getHardwareVersion(self._handle)

    # @function get_get_serial_number
    # @brief Request the device serial number (no direct return).
    # @param self The Graphiti instance.
    # @return None
    def get_get_serial_number(self):
        self._lib.graphiti_getSerialNumber(self._handle)

    # @function get_get_battery_status
    # @brief Request the battery status from the device.
    # @param self The Graphiti instance.
    # @return None
    def get_get_battery_status(self):
        self._lib.graphiti_getBatteryStatus(self._handle)

    # @function get_resolution_information
    # @brief Request the display resolution information.
    # @param self The Graphiti instance.
    # @return None
    def get_resolution_information(self):
        self._lib.graphiti_getResolutionInformation(self._handle)

    # @function get_device_orientation
    # @brief Request the device orientation (portrait/landscape etc.).
    # @param self The Graphiti instance.
    # @return None
    def get_device_orientation(self):
        self._lib.graphiti_getDeviceOrientation(self._handle)

    # @function get_height_information
    # @brief Request tactile height information for the device.
    # @param self The Graphiti instance.
    # @return None
    def get_height_information(self):
        self._lib.graphiti_getHeightInformation(self._handle)

    # @function get_device_name
    # @brief Request the human-readable device name.
    # @param self The Graphiti instance.
    # @return None
    def get_device_name(self):
        self._lib.graphiti_getDeviceName(self._handle)

    # Display Commands

    # @function update_display
    # @brief Update the full display buffer with raw bytes.
    # @param self The Graphiti instance.
    # @param data Byte buffer for display.
    # @return None
    def update_display(self, data: bytes):
        buf = (c_uint8 * len(data))(*data)
        self._lib.graphiti_updateDisplay(self._handle, buf, len(data))

    # @function set_display
    # @brief Apply the prepared buffer to the physical display.
    # @param self The Graphiti instance.
    # @return None
    def set_display(self):
        self._lib.graphiti_setDisplay(self._handle)

    # @function clear_display
    # @brief Clear the current display contents immediately.
    # @param self The Graphiti instance.
    # @return None
    def clear_display(self):
        self._lib.graphiti_clearDisplay(self._handle)

    # @function update_single_pixel
    # @brief Update a single pixel's height and blink attributes.
    # @param self The Graphiti instance.
    # @param row Row index.
    # @param column Column index.
    # @param height Pin height.
    # @param blink_rate Blink rate.
    # @return None
    def update_single_pixel(self, row: int, column: int, height: int, blink_rate: int):
        self._lib.graphiti_updateSinglePixel(self._handle, row, column, height, blink_rate)

    # @function update_single_row
    # @brief Update a single row with the provided byte row buffer.
    # @param self The Graphiti instance.
    # @param row Row index.
    # @param data Byte buffer for row.
    # @return None
    def update_single_row(self, row: int, data: bytes):
        buf = (c_uint8 * len(data))(*data)
        self._lib.graphiti_updateSingleRow(self._handle, row, buf, len(data))

    # @function update_single_column
    # @brief Update a single column with the provided byte column buffer.
    # @param self The Graphiti instance.
    # @param column Column index.
    # @param data Byte buffer for column.
    # @return None
    def update_single_column(self, column: int, data: bytes):
        buf = (c_uint8 * len(data))(*data)
        self._lib.graphiti_updateSingleColumn(self._handle, column, buf, len(data))

    # Status calls

    # @function get_all_pixels_position_status
    # @brief Query the device for status of all pixel positions.
    # @param self The Graphiti instance.
    # @return None
    def get_all_pixels_position_status(self):
        self._lib.graphiti_getAllPixelsPositionStatus(self._handle)

    # @function get_single_pixel_position_status
    # @brief Query status of a single pixel position (expects args).
    # @param self The Graphiti instance.
    # @return None
    def get_single_pixel_position_status(self):
        self._lib.graphiti_getSinglePixelPositionStatus(self._handle)

    # @function get_single_row_pixel_position_status
    # @brief Query status of a single row (expects row arg).
    # @param self The Graphiti instance.
    # @return None
    def get_single_row_pixel_position_status(self):
        self._lib.graphiti_getSingleRowPixelPositionStatus(self._handle)

    # @function get_single_column_pixel_position_status
    # @brief Query status of a single column (expects column arg).
    # @param self The Graphiti instance.
    # @return None
    def get_single_column_pixel_position_status(self):
        self._lib.graphiti_getSingleColumnPixelPositionStatus(self._handle)

    # @function show_message
    # @brief Render a textual message on the device with optional flags.
    # @param self The Graphiti instance.
    # @param message Message string.
    # @param select_flags Byte flags for selection.
    # @param blink_rate Blink rate.
    # @return None
    def show_message(self, message: str, select_flags: bytes, blink_rate: int):
        flags_buf = (c_uint8 * len(select_flags))(*select_flags)
        self._lib.graphiti_showMessage(
            self._handle,
            message.encode('utf-8'),
            flags_buf,
            len(select_flags),
            blink_rate
        )

    # @function set_cursor
    # @brief Configure the text/graphic cursor on the device.
    # @param self The Graphiti instance.
    # @param row Row index.
    # @param column Column index.
    # @param height Cursor height.
    # @param length Cursor length.
    # @param blink_rate Blink rate.
    # @return None
    def set_cursor(self, row: int, column: int, height: int, length: int, blink_rate: int):
        self._lib.graphiti_setCursor(self._handle, row, column, height, length, blink_rate)

    # @function send_image_interruptible
    # @brief Send an image to device in interruptible (non-blocking) mode.
    # @param self The Graphiti instance.
    # @param name Image name.
    # @param filepath Path to image file.
    # @return None. Raises FileNotFoundError if file not found.
    def send_image_interruptible(self, name: str, filepath: str):
        if not os.path.exists(filepath):
            raise FileNotFoundError(f"Image file not found: {filepath}")
        self._lib.graphiti_sendImageInterruptible(
            self._handle,
            name.encode('utf-8'),
            filepath.encode('utf-8')
        )

    # @function send_image_blocking
    # @brief Send an image to the device and block until transfer completes.
    # @param self The Graphiti instance.
    # @param name Image name.
    # @param filepath Path to image file.
    # @return None. Raises FileNotFoundError if file not found.
    def send_image_blocking(self, name: str, filepath: str):
        if not os.path.exists(filepath):
            raise FileNotFoundError(f"Image file not found: {filepath}")
        self._lib.graphiti_sendImageBlocking(
            self._handle,
            name.encode('utf-8'),
            filepath.encode('utf-8')
        )
    
    # @function set_key_event
    # @brief Enable or disable key event generation on the device.
    # @param self The Graphiti instance.
    # @param enabled True to enable, False to disable.
    # @return None
    def set_key_event(self, enabled: bool):
        self._lib.graphiti_setKeyEvent(self._handle, enabled)

    # @function set_touch_event
    # @brief Enable or disable touch event generation on the device.
    # @param self The Graphiti instance.
    # @param enabled True to enable, False to disable.
    # @return None
    def set_touch_event(self, enabled: bool):
        self._lib.graphiti_setTouchEvent(self._handle, enabled)

    # @function get_last_touch_point_status
    # @brief Query for the last touch point status from the device.
    # @param self The Graphiti instance.
    # @return None
    def get_last_touch_point_status(self):
        self._lib.graphiti_getLastTouchPointStatus(self._handle)

    # @function vibrator_control_command
    # @brief Control the device vibrator motor with frequency/duty/duration.
    # @param self The Graphiti instance.
    # @param frequency Vibration frequency.
    # @param duty_cycle Duty cycle.
    # @param duration_ms Duration in milliseconds.
    # @return None
    def vibrator_control_command(self, frequency: int, duty_cycle: int, duration_ms: int):
        self._lib.graphiti_vibratorControlCommand(self._handle, frequency, duty_cycle, duration_ms)

    # @function get_date_and_time
    # @brief Request the current date & time from the device (no return).
    # @param self The Graphiti instance.
    # @return None
    def get_date_and_time(self):
        self._lib.graphiti_getDateAndTime(self._handle)

    # @function set_date_and_time
    # @brief Set the device internal date & time.
    # @param self The Graphiti instance.
    # @param day Day of month.
    # @param month Month.
    # @param year Year.
    # @param hour Hour.
    # @param minute Minute.
    # @param second Second.
    # @return None
    def set_date_and_time(self, day: int, month: int, year: int, hour: int, minute: int, second: int):
        self._lib.graphiti_setDateAndTime(self._handle, day, month, year, hour, minute, second)

