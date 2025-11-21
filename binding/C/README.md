Build the code in this directory with:

.\build.ps1

Run with:

.\test.exe

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