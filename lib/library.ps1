<#
 ******************************************************************************
 * @file    library.ps1
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   Library build helper for Graphiti.
 *
 * @details
 * Helper script invoked by higher-level install scripts to build the
 * Graphiti library. Uses the configured vcpkg toolchain and CMake build
 * steps for packaging the library artifacts.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 ******************************************************************************
#>

param(
    [string]$Generator,
    [string]$Compiler
)

# Commands for building library with library CMakeLists.txt
Remove-Item -Recurse -Force .\build\ -ErrorAction SilentlyContinue

$cmakeArgs = @("-S", ".", "-B", "build", "-DCMAKE_BUILD_TYPE=Release")

if ($Generator) {
    $cmakeArgs += "-G"
    $cmakeArgs += $Generator
}

if ($Compiler) {
    $cmakeArgs += "-DCMAKE_CXX_COMPILER=$Compiler"
}

cmake @cmakeArgs

cmake --build build --config Release

# Install build at user graphiti folder
cmake --install build --config Release --prefix "$env:USERPROFILE\graphiti"


$graphitiPath = "$env:USERPROFILE\graphiti"
$envPath = [Environment]::GetEnvironmentVariable("PATH", "User")

if (-not $envPath.Contains($graphitiPath)) {
    [Environment]::SetEnvironmentVariable("PATH", "$envPath;$graphitiPath", "User")
    Write-Host "`nGraphiti path added to user PATH. You may need to restart your terminal."
} else {
    Write-Host "`nGraphiti is already in user PATH."
}