## java ##

- This folder contains everything used for the Java binding.

- The Java binding uses the C++ portion of the library. Due to the nature of reinterpret cast, all required headers are included via Extension.hpp in internalhandle.hpp. Therefore, this folder requires access to all necessary header files.

- Using build.ps1, the Java library can be built assuming the Graphiti C++ library has already been built on the system.

- Due to the complexity of the JNI wrapper, Java code must be executed either within this folder (using compile_run.ps1) or within the Java_vcp folder (using runJava.ps1 or similar commands). This is required because the library files must remain in the binding/jgraphiti directory structure for JNI function naming to work correctly.

- The `binding/jgraphiti` folder provides the Java HID (Human Interface Device) implementation for communication with the Graphiti device.

- The `Java_vcp` folder contains a Java VCP (Virtual COM Port) test example implementation.

- It is possible to separate the binding folder (containing jgraphiti and required headers) and install it in a user-level directory. Automating this setup would be a useful improvement to abstract library dependencies from the developer workspace.

- As a minimum, the jgraphiti folder has been made separable from the working directory used in Java_vcp.

- compile_run.ps1 was used initially for testing the Java wrapper before introducing runJava.ps1 in the Java_vcp folder.

Library details
- The Java library does not support the functions setConnection or createWithConnection due to the difficulties of passing the Connection object. This means that connections with the device must be made with startUp where the connection handled internally. Functions like setConnection and createWithConnection could be made that use strings to keep this functionality. But since the connection classes for  Bluetooth is not currently developed, I felt it best to keep things this way for now until a function can be made in Connection that would work for all three connection types (VCP, HID, and Bluetooth). Since this change to Connection would not add any funcitonality without Bluetooth support I do not see it as a high priorty atm.

## Running the Project ##
1. Check Java Version
   - javac --version (Java 20+ is recommended.)

2. Navigate to Project Root
   - cd Graphiti-API-Library

3. Install Dependencies via vcpkg
   - .\scripts\install_library_vcpkg.ps1

4. Navigate to Library Folder
   - cd lib 

5. Build C++ Library
   - .\library.ps1 -Generator "Ninja" -Compiler "g++"

6. Navigate to Java Binding Directory
   - cd Graphiti-API-Library\binding\jgraphiti

7. Build Java Bindings
   - .\build.ps1

8. Copy Required Files
   - .\copy-files.ps1
   - Before running the project, make sure the following DLL files are present in the jgraphiti folder:
   - libGraphiti.dll        
   - libGraphitiJNI.dll   
   - Graphiti_C.dll         
   - hidapi.dll  

9. Compile and Run Java Example
   - compile_run.ps1

## Include path ##  
- Add these to your include path with Ctrl+Shift+P
- ${vcpkgRoot}/x64-windows/include
- C:\Program Files\Java\jdk-20\include
- C:\Program Files\Java\jdk-20\include\win32
- ${env:USERPROFILE}\graphiti\include

## Dependencies ##
- hidapi.dll
- hidapi.h
- hidapi_winapi.h
- libGraphiti_C.dll (The C wrapper only works with Single Config of a .dll)
- libGraphiti_C.dll.a
For Multi-Config
- Graphiti_C.dll
- Graphiti_C.lib
Header files of library 
    - Add to configuration or include path(Ctrl + Shift + P for C/C++):
    ${env:USERPROFILE}\graphiti\include

## File Structure ##
- build
    - Folder of current build
- binding_jgraphiti_Graphiti.h
    - Generated file from "javac -h . Graphiti.java"
- build.ps1
    - Deletes all dll files except hidapi.dll and pulls newest version from graphiti folder under user
    - Deletes all .class files and rebuilds them from .java files for library
- CMakeLists.txt
    - CMake for the java library
- compile_run.ps1
    - Compiles all .java files then runs them with Main.java
- copy-dll.ps1
    - Copies the C++ dll to the directory from graphit folder under user
- Graphiti-JNI.cpp
    - JNI wrapper for Graphiti C++ library
- Graphiti.class
    - Compiled from Graphiti.java
- Graphiti.java
    - Graphiti header for JNI
- Graphiti$DrawEvent.class
    - Class file for DrawEvent
- Graphiti$PinInfo.class
    - Class file for PinInfo class
- hidapi_winapi.h
    - header file for hidapi
- hidapi.dll
    - dll file for hidapi library for connection over HID
- hidapi.h
    - header file for hidapi
- internal_handle.hpp
    - header file to have a handle for the Graphiti C++ class
- libGraphiti.dll
    - C++ compiled Graphiti dll
- libGraphiti.dll.a
    - header file for C++ compiled Graphiti dll
- libGraphitJNI.dll
    - compiled JNI dll
- libGraphitiJNI.dll.a
    - header for compiled JNI dll
- Main.class
    - Compiled Main.java
- Main.java
    - Test code for JNI graphiti library

## Java Test Coverage (HID)

The Java wrapper (`binding/jgraphiti/Main.java`) includes a test runner that validates core Graphiti API functionality using the HID connection via JNI.

The following tests and operations are implemented:

### Connection Test
- startUpHID (supports both Graphiti and Graphiti Plus using VID/PID)
- Device connection validation

### Initialization
- startResponseThread  
- setKeyEvent  
- setTouchEvent  

### Display Tests
- update_Display_Functionality  
- set_Display_Functionality  
- clear_Display_Functionality  
- set_Cursor_Functionality  

### Device Info Tests
- get_Software_Version  
- get_Hardware_Version  
- get_Serial_Number  
- get_Battery_Status  
- get_Date_And_Time  
- get_Device_Name  
- get_Resolution_Information  

### Pixel Status Tests
- get_ALL_Pixels_Position_Status  
- get_Single_Pixel_Position_Status  

### Key Event Test
- get_Next_KeyEvent_Functionality  

### Cleanup
- stopResponseThread  
- shutDownHID  
- close connection  

These tests demonstrate the complete workflow of device connection, interaction, event handling, and shutdown using the Java JNI wrapper.