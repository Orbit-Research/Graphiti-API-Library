java

- This folder contains everything used for the java binding

- The java binding uses the C++ portion of the library. Due to the nature of reinterpret cast I was required to include all the headers by including Extension.hpp in internalhandle.hpp. This means that this folder cannot funciton without having a folder of all the headers

- Using build.ps1 one can build the java library assuming the Graphiti library has been built on the computer already

- Due to the complexity of the JNI wrapper, Code to run with java must run in the examplej folder or in this folder with compile_run.ps1 or something similar of running javac command followed by a command that runs the java class files with the library. This is because the files of the library must remain in the same directory setup of binding/jgraphiti in order to function due ot JNI function naming

- However, I assume this also means that the binding folder itself or a folder that is called binding can simply hold the jgraphiti folder and this as a whole(with the header files) could be installed under a user. This kind of improvement would be a very helpful script to write so that the library contents can be abstracted away as to not take up space in a developer's working directory

- As a minimum I have made the jgraphiti folder able to be seperate from the working directory folder of examplej

- compile_run.ps1 was use by me before I made runJava.ps1 in the examplej folder to test the java wrapper

Library details
- The Java library does not support the functions setConnection or createWithConnection due to the difficulties of passing the Connection object. This means that connections with the device must be made with startUp where the connection handled internally. Functions like setConnection and createWithConnection could be made that use strings to keep this functionality. But since the connection classes for  Bluetooth is not currently developed, I felt it best to keep things this way for now until a function can be made in Connection that would work for all three connection types (VCP, HID, and Bluetooth). Since this change to Connection would not add any funcitonality without Bluetooth support I do not see it as a high priorty atm.

## Usage ##

Please make sure to follow the Graphiti Setup guide document in the documents folder before contining.

Copy dlls and build using build.ps1 by running from the binding\jgraphiti directory
If using Multi-Config ei Graphiti.lib and Graphiti.dll then change the top of Graphiti-JNI.cpp on line 7 to use Graphiti.lib instead of libGraphiti.dll.a 
If using Single-Config use libGraphiti.dll.a on line 7
Note: The Java wrapper requires all header files of the library thus it cannot be run without them in the current director and or added to the visual studio include path.
Then compile and run using compile_run.ps1

## Include path ##
Add these to your include path with Ctrl+Shift+P
    ${vcpkgRoot}/x64-windows/include
    C:\Program Files\Java\jdk-20\include
    C:\Program Files\Java\jdk-20\include\win32

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
