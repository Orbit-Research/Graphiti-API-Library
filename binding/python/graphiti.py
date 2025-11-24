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
    def __init__(self):
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

    def graphiti_create(self):
        self._handle = self._lib.graphiti_create()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self._lib.graphiti_destroy(self._handle)

    # VCP control
    def start_vcp(self, port: str, key_events: bool = True, touch_events: bool = True) -> bool:
        return self._lib.graphiti_startUpVCP(
            self._handle,
            port.encode('ascii'),
            c_bool(key_events),
            c_bool(touch_events)
        )

    def shutdown_vcp(self, key_events: bool = True, touch_events: bool = True):
        self._lib.graphiti_shutDownVCP(
            self._handle,
            c_bool(key_events),
            c_bool(touch_events)
        )

    # VCP control
    def start_hid(self, vendor_id: int, product_id: int, key_events: bool = True, touch_events: bool = True) -> bool:
        return self._lib.graphiti_startUpHID(
            self._handle,
            c_uint16(vendor_id),
            c_uint16(product_id),
            c_bool(key_events),
            c_bool(touch_events)
        )

    def shutdown_hid(self, key_events: bool = True, touch_events: bool = True):
        self._lib.graphiti_shutDownHID(
            self._handle,
            c_bool(key_events),
            c_bool(touch_events)
        )

    # Extension screen management
    def get_index(self, row: int, col: int) -> int:
        return self._lib.graphiti_index(self._handle, row, col)

    def set_pin(self, row: int, col: int, height: int):
        self._lib.graphiti_setPin(self._handle, row, col, height)

    def clear_screen(self):
        self._lib.graphiti_clearScreen(self._handle)

    def sleep(self, seconds: int):
        self._lib.graphiti_sleep(self._handle, seconds)

    def start_response_thread(self):
        self._lib.graphiti_startResponseThread(self._handle)

    def stop_response_thread(self):
        self._lib.graphiti_stopResponseThread(self._handle)

    # Event handling
    def get_next_output_event(self) -> Optional[str]:
        buf = create_string_buffer(512) 

        self._lib.graphiti_getNextOutputEvent(self._handle, buf, len(buf))

        output = buf.value.decode('utf-8')

        return output if output else None

    def get_next_display_status_event(self) -> list[int] | None:
        MAX_LEN = 2400
        buffer = (c_int * MAX_LEN)()

        # Set argtypes/restype (ideally only once)
        self._lib.graphiti_getNextDisplayStatusEvent.argtypes = [c_void_p, POINTER(c_int),c_int]
        self._lib.graphiti_getNextDisplayStatusEvent.restype = c_int

        count = self._lib.graphiti_getNextDisplayStatusEvent(self._handle, buffer, MAX_LEN)

        if count <= 0:
            return None

        return list(buffer[:count])

    def get_next_key_event(self) -> list[str] | None:
        MAX_KEYS = 14
        MAX_STRING_LEN = 64

        # Step 1: Allocate Python string buffers
        string_buffers = [create_string_buffer(MAX_STRING_LEN) for _ in range(MAX_KEYS)]

        # Step 2: Create char** (array of pointers to buffers)
        key_array_type = c_char_p * MAX_KEYS
        key_array = key_array_type(*(cast(buf, c_char_p) for buf in string_buffers))

        # Set argtypes/restype
        self._lib.graphiti_getNextKeyEvent.argtypes = [c_void_p, POINTER(c_char_p), c_int]
        self._lib.graphiti_getNextKeyEvent.restype = c_int

        count = self._lib.graphiti_getNextKeyEvent(self._handle, key_array, MAX_STRING_LEN)

        if count <= 0:
            return None

        return [buf.value.decode("utf-8") for buf in string_buffers[:count]]

    def get_next_gesture_event(self) -> Optional[str]:
        buf = create_string_buffer(512) 

        self._lib.graphiti_getNextGestureEvent(self._handle, buf, len(buf))

        output = buf.value.decode('utf-8')
        
        return output if output else None

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
    def send_ack(self):
        self._lib.graphiti_sendACK(self._handle)

    def send_nack(self):
        self._lib.graphiti_sendNACK(self._handle)

    def get_software_version(self):
        self._lib.graphiti_getSoftwareVersion(self._handle)

    def get_hardware_version(self):
        self._lib.graphiti_getHardwareVersion(self._handle)

    def get_get_serial_number(self):
        self._lib.graphiti_getSerialNumber(self._handle)

    def get_get_battery_status(self):
        self._lib.graphiti_getBatteryStatus(self._handle)

    def get_resolution_information(self):
        self._lib.graphiti_getResolutionInformation(self._handle)

    def get_device_orientation(self):
        self._lib.graphiti_getDeviceOrientation(self._handle)

    def get_height_information(self):
        self._lib.graphiti_getHeightInformation(self._handle)

    def get_device_name(self):
        self._lib.graphiti_getDeviceName(self._handle)

    # Display Commands

    def update_display(self, data: bytes):
        buf = (c_uint8 * len(data))(*data)
        self._lib.graphiti_updateDisplay(self._handle, buf, len(data))

    def set_display(self):
        self._lib.graphiti_setDisplay(self._handle)

    def clear_display(self):
        self._lib.graphiti_clearDisplay(self._handle)

    def update_single_pixel(self, row: int, column: int, height: int, blink_rate: int):
        self._lib.graphiti_updateSinglePixel(self._handle, row, column, height, blink_rate)

    def update_single_row(self, row: int, data: bytes):
        buf = (c_uint8 * len(data))(*data)
        self._lib.graphiti_updateSingleRow(self._handle, row, buf, len(data))

    def update_single_column(self, column: int, data: bytes):
        buf = (c_uint8 * len(data))(*data)
        self._lib.graphiti_updateSingleColumn(self._handle, column, buf, len(data))

    # Status calls

    def get_all_pixels_position_status(self):
        self._lib.graphiti_getAllPixelsPositionStatus(self._handle)

    def get_single_pixel_position_status(self):
        self._lib.graphiti_getSinglePixelPositionStatus(self._handle)

    def get_single_row_pixel_position_status(self):
        self._lib.graphiti_getSingleRowPixelPositionStatus(self._handle)

    def get_single_column_pixel_position_status(self):
        self._lib.graphiti_getSingleColumnPixelPositionStatus(self._handle)



    def show_message(self, message: str, select_flags: bytes, blink_rate: int):
        flags_buf = (c_uint8 * len(select_flags))(*select_flags)
        self._lib.graphiti_showMessage(
            self._handle,
            message.encode('utf-8'),
            flags_buf,
            len(select_flags),
            blink_rate
        )

    def set_cursor(self, row: int, column: int, height: int, length: int, blink_rate: int):
        self._lib.graphiti_setCursor(self._handle, row, column, height, length, blink_rate)

    def send_image_interruptible(self, name: str, filepath: str):
        if not os.path.exists(filepath):
            raise FileNotFoundError(f"Image file not found: {filepath}")
        
        self._lib.graphiti_sendImageInterruptible(
            self._handle,
            name.encode('utf-8'),
            filepath.encode('utf-8')
        )

    def send_image_blocking(self, name: str, filepath: str):
        if not os.path.exists(filepath):
            raise FileNotFoundError(f"Image file not found: {filepath}")
        
        self._lib.graphiti_sendImageBlocking(
            self._handle,
            name.encode('utf-8'),
            filepath.encode('utf-8')
        )
    
    def set_key_event(self, enabled: bool):
        self._lib.graphiti_setKeyEvent(self._handle, enabled)

    def set_touch_event(self, enabled: bool):
        self._lib.graphiti_setTouchEvent(self._handle, enabled)

    def get_last_touch_point_status(self):
        self._lib.graphiti_getLastTouchPointStatus(self._handle)

    def vibrator_control_command(self, frequency: int, duty_cycle: int, duration_ms: int):
        self._lib.graphiti_vibratorControlCommand(self._handle, frequency, duty_cycle, duration_ms)

    def get_date_and_time(self):
         self._lib.graphiti_getDateAndTime(self._handle)

    def set_date_and_time(self, day: int, month: int, year: int, hour: int, minute: int, second: int):
        self._lib.graphiti_setDateAndTime(self._handle, day, month, year, hour, minute, second)
    
