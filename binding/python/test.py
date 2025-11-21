# test.py
from graphiti import Graphiti
import time

# Load the library
handle = Graphiti()

handle.graphiti_create()

# if not handle.start_vcp("COM4", False, False):
#     raise RuntimeError("Connection failed")

if not handle.start_hid(0x1FC9, 0x8217, False, False):
     raise RuntimeError("Connection failed")

handle.get_software_version()
time.sleep(2)

strOpt = handle.get_next_output_event()
if strOpt is not None:
    print(strOpt)

time.sleep(2)

# handle.shutdown_vcp(False, False)
handle.shutdown_hid(False, False)
