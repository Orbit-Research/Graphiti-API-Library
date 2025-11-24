cmake_minimum_required(VERSION 3.12)

set(CMAKE_TOOLCHAIN_FILE "$ENV{USERPROFILE}/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(GraphitiJNI)

# Find Java and JNI
find_package(Java REQUIRED)
find_package(JNI REQUIRED)

# asio
find_package(asio REQUIRED)

# Add your JNI library
add_library(GraphitiJNI SHARED 
    Graphiti-JNI.cpp
    # Add other source files here
)

set(GRAPHITI_ROOT "${CMAKE_SOURCE_DIR}/../..")

# Include directories
target_include_directories(GraphitiJNI PRIVATE
    ${JNI_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}  # For local headers
    ${GRAPHITI_ROOT}/lib/include
    # Add other include paths here
)

target_link_libraries(GraphitiJNI PUBLIC 
    asio::asio
    ws2_32
)

# Link against your existing lib_Graphiti
target_link_libraries(GraphitiJNI PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/libGraphiti.dll.a  # Links against libGraphiti.dll/.so
)

# Copy the DLL to build directory (Windows)
if(WIN32)
    add_custom_command(TARGET GraphitiJNI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/libGraphiti.dll
            ${CMAKE_BINARY_DIR}/libGraphiti.dll
    )
endif()