## python wrapper ##
- Any python code can use the python wrapper by running with the libGraphiti_C.dll and graphiti.py

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

### Running the Project ##

1. Navigate to Project Root 
   cd Graphiti-API-Library 

2. Install Dependencies via vcpkg
   .\scripts\install_library_vcpkg.ps1 

3. Navigate to Library Folder
   cd Graphiti-API-Library\lib 

4. Build the C++ Library
   .\library.ps1 

5. Navigate to Python Binding Directory
   cd binding/python 

6. Copy Required Runtime Files
   .\copy-files.ps1 
   Before running the project, make sure the following DLL files are present in the Python folder:
   -Graphiti_C.dll (libGraphiti_C.dll)
   -libGraphiti.dll      
   -hidapi.dll  

7. Run Python Test Script
   python test.py.

**Switch to VCP mode**:
- In `test.py`, comment HID block and uncomment VCP block.
- python test.py.

