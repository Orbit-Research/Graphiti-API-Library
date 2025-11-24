python wrapper
- Any python code can use the python wrapper by running with the libGraphiti_C.dll and graphiti.py

## Usage ##

Please make sure to follow the Graphiti Setup guide document in the documents folder before contining.

Copy files from the compiled library on your computer using using copy-files.ps1 by running from the binding\python directory
Run using run.ps1 or a similar command to run your code

## File Structure ##
- copy-dll-C.ps1
    - Script to copy the libGraphiti_C.dll and hidapi.dll files to the current directory from graphit folder under user
- graphiti.py
    - Wrapper for the Graphiti library using ctypes
- hidapi.dll
    - dll file for hidapi to enable HID
    - Dependency of libGraphiti_C.dll
- libGraphiti.dll
    - Compiled library for the Graphiti
- Graphiti.dll
    - Compiled library for the Graphiti from Mult-Config
- run.ps1
    - Run script for convience
- test.py
    - File for showing the use of the graphiti python library

## Dependencies ##
- hidapi.dll
- hidapi.h
- hidapi_winapi.h
- libGraphiti_C.dll (The C wrapper only works with Single Config of a .dll)
- libGraphiti_C.dll.a
For Multi-Config
- Graphiti_C.dll
- Graphiti_C.lib