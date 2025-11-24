# build.ps1 - CMake-based build

# Clean previous builds
# Don't delte hidapi.dll
Get-ChildItem -Path . -Filter *.dll -Recurse |
Where-Object { $_.Name -ne 'hidapi.dll' } |
Remove-Item -Force -ErrorAction SilentlyContinue

# Copy dlls
.\copy-files.ps1

# Remove JNI headers
Remove-Item -Force -Recurse -ErrorAction SilentlyContinue .\Graphiti.class, '.\Graphiti$DrawEvent.class', '.\Graphiti$PinInfo.class', .\binding_jGraphiti_Graphiti.h

# Compile JNI headers
javac Graphiti.java

# Generate JNI headers
javac -h . Graphiti.java

# Configure and build
cmake -B build -G "Ninja" `
    -DCMAKE_BUILD_TYPE=Release `
    -DJava_HOME="$env:JAVA_HOME"

cmake --build build

# Optional: Copy results to current directory
Copy-Item -Path build/libGraphitiJNI.* -Destination .