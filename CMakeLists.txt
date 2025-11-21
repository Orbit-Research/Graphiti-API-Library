cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 23)

# Set vcpkg toolchain (adjust path as needed)
set(CMAKE_TOOLCHAIN_FILE "$ENV{USERPROFILE}/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_CXX_COMPILER "g++")
add_definitions(-D_WIN32_WINNT=0x0601)

project(Graphiti_API_Library_0_1)

find_package(asio CONFIG REQUIRED)
find_package(hidapi CONFIG REQUIRED)

# Set library sources
set(GRAPHITI_SOURCES
    lib/src/API.cpp
    lib/src/Extension.cpp
    lib/src/Connection/Connection_VCP.cpp
    lib/src/Connection/Connection_HID.cpp
)

# Create the library SHARED
add_library(Graphiti SHARED ${GRAPHITI_SOURCES})

target_compile_definitions(Graphiti PRIVATE BUILDING_GRAPHITI_DLL)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories for the library
target_include_directories(Graphiti PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/include/
)

# Find and link dependencies
target_link_libraries(Graphiti PUBLIC 
    asio::asio
    ws2_32
    hidapi::hidapi
    hidapi::winapi
    hidapi::include
)

# Add Debug Mode
target_compile_definitions(Graphiti PRIVATE $<$<CONFIG:Debug>:DEBUG>)

# Create executable
function(add_graphiti_exe name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE Graphiti)
endfunction()

# Add executables

# add_graphiti_exe(ExtensionTest main/ExtensionTest.cpp)

set(TEST_SOURCES
    TestFolder/main.cpp
    TestFolder/TestHarness.cpp
    TestFolder/TestHarness.hpp
    TestFolder/Tests.cpp
    TestFolder/Tests.hpp
)
add_executable(Testing ${TEST_SOURCES})

target_link_libraries(Testing PRIVATE Graphiti)