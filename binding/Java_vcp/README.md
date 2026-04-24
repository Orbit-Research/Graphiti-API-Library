## Java VCP
Place any .java files into this folder and run the runJava.ps1 command and the .java files will be compiled and run with the library.

The `java_vcp` folder contains a Java VCP test example for communication with the Graphiti device over Virtual COM Port (VCP).

Make sure to do: "import binding.jgraphiti.Graphiti;" in any .java file(s) that use the graphiti library

## Java library dependencies ##
- hidapi_winapi.h
- hidapi.h
- hidapi.dll
- libGraphiti.dll
- libGraphiti.dll.a
- libGraphitiJNI.dll
- libGraphitiJNI.dll.a
- All Graphiti .class files
- Header files of the cpp library (To the best of my knowledge)

## Java Test Coverage (VCP)

The Java VCP example (`binding/Java_vcp/Main.java`) demonstrates basic communication with the Graphiti device over a Virtual COM Port (VCP) using the Java JNI wrapper.

The following operations are implemented:

### Connection Test
- startUpVCP (connects to device using specified COM port)
- Device connection initialization over VCP

### Basic Query Test
- get_Software_Version  

### Event Handling
- get_Next_Output_Event  

### Utility
- sleep (used for timing and response handling)

### Cleanup
- shutDownVCP  
- close connection  

This example provides a minimal workflow for establishing a VCP connection, sending a command, receiving a response, and properly shutting down the device connection.