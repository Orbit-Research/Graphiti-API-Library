## Running the Project ##

1. Build the Graphiti library using GCC/Ninja so it matches the compiler `build.ps1` uses to link `test.exe`:

- cd lib
- .\library.ps1 -Generator "Ninja" -Compiler "g++"

2. Copy the built DLLs, headers, and hidapi dependencies into this folder:

- cd ..\binding\C
- .\copy-files.ps1

3. Build the code in this directory with:
- .\build.ps1
- Before running the project, make sure the following DLL files are present in the C folder:
- Graphiti.dll (or libGraphiti.dll)
- libGraphiti_C.dll
- hidapi.dll
4. Run with:
- .\test.exe

> Do not mix an MSVC-built `Graphiti_C.dll`/`Graphiti_C.lib` into this folder — `build.ps1` links with GCC, so the library must also be built with GCC (see Troubleshooting below if you hit "Entry Point Not Found").

## Troubleshooting ##

### "Entry Point Not Found" when running test.exe ###
This happens when the core library (`libGraphiti.dll`) and the C wrapper (`libGraphiti_C.dll`) were built with a **different compiler/toolchain than the one used to build `test.exe`**.

- `build.ps1` compiles `main.c` using **GCC (MinGW)**.
- If `lib/library.ps1` is run without `-Generator`/`-Compiler` arguments, CMake defaults to **Visual Studio (MSVC)** on Windows, producing an MSVC-built `Graphiti.dll`/`Graphiti_C.dll` instead.
- GCC and MSVC use different C++ ABIs (name mangling, STL layout), so a GCC-linked `test.exe` cannot correctly call into an MSVC-built DLL for functions with C++ types in their internal implementation (e.g. `showMessageOnBraille`, which uses `std::vector<unsigned char>`).

**Fix:** build the library with the same toolchain (GCC) used for the C example, then re-run `copy-files.ps1`:
```powershell
cd lib
Remove-Item -Recurse -Force .\build\ -ErrorAction SilentlyContinue
.\library.ps1 -Generator "Ninja" -Compiler "g++"
cd ..\binding\C
.\copy-files.ps1
.\build.ps1
```

`copy-files.ps1` copies both the Single-Config (GCC/Ninja) wrapper (`libGraphiti_C.dll` / `libGraphiti_C.dll.a`) and the Multi-Config (MSVC) wrapper (`Graphiti_C.dll` / `Graphiti_C.lib`) if present. Since `build.ps1` links with GCC, only the `libGraphiti_C.*` files are actually required — don't mix MSVC-built files into the same folder as GCC-built ones.


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