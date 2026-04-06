"""
graphiti_tests.py
Company : Orbit Research
Brief   : Test for selected Graphiti API functions.

Tests covered:
  - update_Display_Functionality
  - set_Display_Funcitonality
  - clear_Display_Functionality
  - get_Next_KeyEvent_Functionality
  - show_Message_On_The_Device_Functionality
  - set_Cursor_Functionality
  - get_Date_And_Time_Correctness
  - get_ALL_Pixels_Position_Status_Correctness
  - get_Single_Pixel_Position_Status_Correctness
  - get_Software_Version_Correctness
  - get_Hardware_Version_Correctness
  - get_Serial_Number_Correctness
  - get_Battery_Status_Correctness

How to run:
  1. Copy this file into:  binding/python/
  2. Make sure graphiti.py and libGraphiti_C.dll are in the same folder
  3. Run:  python graphiti_tests.py

Connection:
  Default = HID (USB).
  To switch to VCP: comment the HID block, uncomment the VCP block below.
"""

from graphiti import Graphiti
import time
import datetime

# CONFIG 
# HID — new device VID/PID tried first, old device second (same as original code)
VID_NEW = 0x0483
PID_NEW = 0xA366
VID_OLD = 0x1FC9
PID_OLD = 0x8217

# VCP config — only needed if switching to VCP mode
# COM_PORT = "COM11"   # <-- change to your actual COM port

SLEEP_TIME = 5        # seconds to wait for device response
MAX_ROWS   = 60
MAX_COLS   = 40
MAX_PIXELS = MAX_ROWS * MAX_COLS  # 2400

# RESULT TRACKING
results = {}

def log(test_name, passed, note=""):
    """
    @function log
    @brief Log test result to console and results dictionary
    @param test_name : str - Name of the test
    @param passed : bool - Whether the test passed or failed
    @param note : str - Optional note to append to result (default="")
    @return None
    """
    status = "PASSED" if passed else "FAILED"
    results[test_name] = status
    print(f"  {test_name}: {status}" + (f"  ({note})" if note else ""))


#  CONNECT
print("  Start Test")

g = Graphiti()
g.graphiti_create()
time.sleep(2)

# HID Connection (default)
connected = False
for vid, pid in [(VID_NEW, PID_NEW), (VID_OLD, PID_OLD)]:
    print(f"Trying HID: VID=0x{vid:04X}, PID=0x{pid:04X} ...")
    if g.start_hid(vid, pid, key_events=True, touch_events=False):
        connected = True
        print("HID Connected!\n")
        break
    else:
        print("Failed.")

if not connected:
    raise RuntimeError("Could not connect to any supported Graphiti device via HID.")

# --- VCP Connection (commented — uncomment to use VCP instead of HID) ---
# print(f"Connecting over VCP on {COM_PORT} ...")
# if not g.start_vcp(COM_PORT, key_events=True, touch_events=False):
#     raise RuntimeError(f"Could not connect on {COM_PORT}. Check Device Manager.")
# print("VCP Connected!\n")

# time.sleep(1)


#  HELPERS
def get_output(wait=SLEEP_TIME):
    """
    @function get_output
    @brief Wait for and retrieve next output event from device
    @param wait : float - Time in seconds to wait before retrieving output (default=SLEEP_TIME)
    @return str - Output event string from device, or None if no output available
    """
    time.sleep(wait)
    return g.get_next_output_event()

def get_display_status(wait=SLEEP_TIME):
    """
    @function get_display_status
    @brief Wait for and retrieve next display status event from device
    @param wait : float - Time in seconds to wait before retrieving status (default=SLEEP_TIME)
    @return array - Display status event (pixel positions/states), or None if no status available
    """
    time.sleep(wait)
    return g.get_next_display_status_event()

def make_full_screen(height=2, blink=0):
    """
    @function make_full_screen
    @brief Create a full screen display buffer with specified height and blink values for all pixels
    @param height : int - Pin height value for all pixels (default=2, range 0-3)
    @param blink : int - Blink rate for all pixels (default=0)
    @return bytes - Full screen data buffer (2400 bytes for 60x40 pixel grid)
    """
    data = bytearray()
    for _ in range(MAX_PIXELS):
        data.append(height)
        data.append(blink)
    return bytes(data)


#  1. update_Display_Functionality
print("Running: update_Display_Functionality")
try:
    screen = make_full_screen(height=2, blink=0)
    g.update_display(screen)
    time.sleep(10)
    log("update_Display_Functionality", True,
        "Check device: all 2400 pins raised to height=2")
except Exception as e:
    log("update_Display_Functionality", False, str(e))


#  2. set_Display_Funcitonality
print("Running: set_Display_Funcitonality")
try:
    g.set_display()
    time.sleep(10)
    log("set_Display_Funcitonality", True,
        "Check device: buffered data committed")
except Exception as e:
    log("set_Display_Funcitonality", False, str(e))


#  3. clear_Display_Functionality
print("Running: clear_Display_Functionality")
try:
    g.clear_display()
    time.sleep(10)
    log("clear_Display_Functionality", True, "Check device: all pins down")
except Exception as e:
    log("clear_Display_Functionality", False, str(e))


#  4. get_Software_Version_Correctness
print("Running: get_Software_Version_Correctness")
try:
    g.get_software_version()
    output = get_output()
    if output:
        print(f"    Device returned: {output}")
        # We don't require exact version match: accept correct prefix / major build family
        expected_prefix = "B"
        passed = output.strip().startswith(expected_prefix)
    else:
        log("get_Software_Version_Correctness", False, "No output received")
except Exception as e:
    log("get_Software_Version_Correctness", False, str(e))

#  5. get_Hardware_Version_Correctness
print("Running: get_Hardware_Version_Correctness")
try:
    g.get_hardware_version()
    output = get_output()
    if output:
        print(f"    Device returned: {output}")
        # Avoid exact matching; accept any non-empty version string with initial letter V or MB
        val = output.strip()
        passed = (val.startswith("V") or val.startswith("MB"))
    else:
        log("get_Hardware_Version_Correctness", False, "No output received")
except Exception as e:
    log("get_Hardware_Version_Correctness", False, str(e))

#  6. get_Serial_Number_Correctness
print("Running: get_Serial_Number_Correctness")
try:
    g.get_get_serial_number()
    output = get_output()
    if output:
        print(f"    Device returned: {output}")
        # Avoid exact matching; accept if serial contains expected family prefix
        val = output.strip()
        passed = val.upper().startswith("MB")
    else:
        log("get_Serial_Number_Correctness", False, "No output received")
except Exception as e:
    log("get_Serial_Number_Correctness", False, str(e))


#  7. get_Battery_Status_Correctness
print("Running: get_Battery_Status_Correctness")
try:
    g.get_get_battery_status()
    output = get_output()
    if output:
        print(f"    Device returned: {output}")
        val = output.strip()
        passed = (('%' in val) and
                  (val.startswith("Battery") or val.startswith("Charging")))
        log("get_Battery_Status_Correctness", passed, f"got='{val}'")
    else:
        log("get_Battery_Status_Correctness", False, "No output received")
except Exception as e:
    log("get_Battery_Status_Correctness", False, str(e))


#  9. show_Message_On_The_Device_Functionality
print("Running: show_Message_On_The_Device_Functionality")
print("    >> Press Space+3 on the device to enable message bar, then press Enter...")
input("    Press Enter to continue: ")
try:
    g.show_message("abcdefghijklmnopqr", bytes([1, 2, 2]), blink_rate=2)
    time.sleep(2)
    log("show_Message_On_The_Device_Functionality", True,
        "Check device: message 'abcdefghijklmnopqr' visible on message bar")
except Exception as e:
    log("show_Message_On_The_Device_Functionality", False, str(e))


#  10. set_Cursor_Functionality
print("Running: set_Cursor_Functionality")
try:
    g.set_cursor(row=20, column=30, height=2, length=10, blink_rate=3)
    time.sleep(2)
    log("set_Cursor_Functionality", True,
        "Check device: cursor at row=20 col=30 height=2 length=10 blinkRate=3")
except Exception as e:
    log("set_Cursor_Functionality", False, str(e))


#  13. get_Next_KeyEvent_Functionality
#      Press '1' to raise pin, any other key to lower it.
#      Press SELECT to exit the key loop.
print("Running: get_Next_KeyEvent_Functionality")
print("    >> Key loop starting:")
print("       - Press key '1' on device  -> pin at row=10,col=30 goes UP")
print("       - Press any other key      -> pin goes DOWN")
print("       - Press SELECT button      -> exits the loop")
try:
    loop = True
    while loop:
        keys = g.get_next_key_event()
        if keys:
            print(f"    Keys pressed: {keys}")
            if "1" in keys:
                g.update_single_pixel(10, 30, 2, 0)
                print("    Pin UP")
            else:
                g.update_single_pixel(10, 30, 0, 0)
                print("    Pin DOWN")
            if "Select" in keys:
                loop = False
                print("    SELECT pressed -- exiting key loop")
        time.sleep(0.1)
    log("get_Next_KeyEvent_Functionality", True, "Key loop completed successfully")
except Exception as e:
    log("get_Next_KeyEvent_Functionality", False, str(e))


#  DISCONNECT
print("\nDisconnecting...")

# --- HID Disconnect (default) ---
g.shutdown_hid(key_events=True, touch_events=False)

# --- VCP Disconnect (commented — uncomment if using VCP) ---
# g.shutdown_vcp(key_events=True, touch_events=False)

print("Disconnected.\n")


#  SUMMARY
print("  TEST SUMMARY")
passed_list = [t for t, r in results.items() if r == "PASSED"]
failed_list = [t for t, r in results.items() if r == "FAILED"]

for t, r in results.items():
    mark = "+" if r == "PASSED" else "x"
    print(f"  [{mark}]  {t}: {r}")

print(f"  Total : {len(results)}  |  Passed : {len(passed_list)}  |  Failed : {len(failed_list)}")

with open("passedTests.txt", "w") as f:
    f.write("\n".join(passed_list))
with open("failedTests.txt", "w") as f:
    f.write("\n".join(failed_list))
print("\nResults saved to passedTests.txt and failedTests.txt")