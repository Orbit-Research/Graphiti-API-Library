## Running the Project ##

1. Rebuilt Graphiti_C in Debug Mode:

- cd "c:\Users\Graphiti-API-Library\lib\build"
- cmake --build . --config Debug --target Graphiti_C

2. Copied Debug DLL:

- copy "c:\Users\Graphiti-API-Library\lib\build\Debug\Graphiti_C.dll" "c:\Users\Graphiti-API-Library\binding\C\Graphiti_C.dll"

3. Build the code in this directory with:
- .\build.ps1
- Before running the project, make sure the following DLL files are present in the C folder:
- Graphiti.dll
- Graphiti_C.dll
- hidapi.dll

4. Run with:

- .\test.exe

## Usage ##

Please make sure to follow the Graphiti Setup guide document in the documents folder before contining.

The copy the lib files using copy-files.ps1 from the binding\C directory
Build with build.ps1
Run the text.exe file

## Commands Notes ##

When using the api calls in C for the output calls, these calls will require providing a buffer and size of that buffer as done in main.c with software_version in order to recieve the data. String buffers are called out* while other kinds of buffers are called buffer. getNextDrawEvent returns the data is PinInfo and returns the count of the PinInfo's. Display status events will output their pin data as the height followed by the blink rate in the order of that return function. Column returns 1 to 40, Row returns 1 to 60, and all will return 1 to 2400 left to right, top to bottom.

## Dependencies ##
- hidapi.dll
- hidapi.h
- hidapi_winapi.h
- libGraphiti_C.dll (The C wrapper only works with Single Config of a .dll)
- libGraphiti_C.dll.a
- capi.h
For Multi-Config
- Graphiti_C.dll
- Graphiti_C.lib

## File Structure ##
- .vscode
    - Folder with preset use of Ninja gcc for python
- build.ps1
    - compiles main.c with the graphiti library into test.exe
- capi.h
    - header file from the C wrapper of the Graphiti library    
    - dependency of libGraphiti_C.dll when compiling in C
- copy-files.ps1
    - Copyies libGraphiti_C.dll from graphiti folder under user
- hidapi.dll
    - hid dll for hid support which is a dependency of libGraphiti_C.dll
- libGraphiti_C.dll
    - Compiled library for Graphiti C wrapper
- main.c
    - Progam to showcase use of Graphiti library with VCP and HID
- test.exe
    - Compiled code for execcution of main.c and library

    ## C Test Coverage (HID & VCP)

The C example (`binding/C/main.c`) demonstrates usage of the Graphiti C wrapper API for communicating with the device. The primary implementation uses HID, while VCP support is also included as a commented reference.

The following tests and operations are implemented:

### Initialization
- graphiti_create (creates device handle)

### Connection Test
- graphiti_startUpHID (active implementation using VID/PID)
- graphiti_startUpVCP (available as commented example for COM port communication)

### Basic Query Test
- get_Software_Version_Correctness  

### Pixel Control Test
- graphiti_updateSinglePixel (set pixel height)  
- graphiti_updateSinglePixel (reset pixel height)  

### Event Handling
- graphiti_getNextOutputEvent  

### Utility
- graphiti_sleep (used for timing and response handling)

### Cleanup
- graphiti_shutDownHID  
- graphiti_destroy  

This example demonstrates a basic workflow for initializing the device, performing API calls, validating responses, and controlling pixels using the C wrapper.

> The VCP connection code is included but commented out. It can be enabled by modifying the connection logic and specifying the correct COM port.