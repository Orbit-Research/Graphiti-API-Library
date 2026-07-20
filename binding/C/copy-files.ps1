<#
 SYNTAX
 Copy required C binding DLLs, headers and dependencies into working directory.

 DESCRIPTION
 Utility used to copy capi header, runtime DLLs and hidapi dependencies into
 the C binding folder for building and running the example C client.

 Copyright (c) 2026 Orbit Research LLC Distributed under project license.
#>

function Copy-ToCurrentDirectory {
    param (
        [string]$FileName,
        [string]$SourcePath
    )

    Write-Host "Deleting existing $FileName (if any...)"
    Remove-Item $FileName -ErrorAction SilentlyContinue

    if (!(Test-Path $SourcePath)) {
        Write-Error "$FileName not found at: $SourcePath"
        return $false
    }

    Copy-Item -Path $SourcePath -Destination $FileName -Force
    Write-Host "Copied $FileName to current directory."
    return $true
}

# Copy Header
$headerSuccess = Copy-ToCurrentDirectory -FileName "capi.h" -SourcePath "$env:USERPROFILE\graphiti\include\Graphiti\CWrapper\capi.h"

# Copy DLL
$dllSuccess = Copy-ToCurrentDirectory -FileName "libGraphiti.dll" -SourcePath "$env:USERPROFILE\graphiti\bin\libGraphiti.dll"
$dllHeader = Copy-ToCurrentDirectory -FileName "libGraphiti.dll.a" -SourcePath "$env:USERPROFILE\graphiti\lib\libGraphiti.dll.a"

# Copy Single-Config C wrapper (GCC/Ninja build). This is what build.ps1 links
# against via `-lGraphiti_C`, so it must match the compiler used for libGraphiti.dll
# above or you will hit "Entry Point Not Found" at runtime.
$cWrapperSuccess = Copy-ToCurrentDirectory -FileName "libGraphiti_C.dll" -SourcePath "$env:USERPROFILE\graphiti\bin\libGraphiti_C.dll"
$cWrapperImplib = Copy-ToCurrentDirectory -FileName "libGraphiti_C.dll.a" -SourcePath "$env:USERPROFILE\graphiti\lib\libGraphiti_C.dll.a"

# Copy Mult-Config DLL (MSVC build). Only needed if you are linking test.exe with
# MSVC (cl.exe) instead of GCC. Do not mix this with the Single-Config files above.
$dllSuccessMC = Copy-ToCurrentDirectory -FileName "Graphiti_C.dll" -SourcePath "$env:USERPROFILE\graphiti\bin\Graphiti_C.dll"
$dllHeaderMC = Copy-ToCurrentDirectory -FileName "Graphiti_C.lib" -SourcePath "$env:USERPROFILE\graphiti\lib\Graphiti_C.lib"

# Copy hidapi
$hidSucessDLL = Copy-ToCurrentDirectory -FileName "hidapi.dll" -SourcePath "$env:USERPROFILE\vcpkg\installed\x64-windows\bin\hidapi.dll"
$hidSucessH = Copy-ToCurrentDirectory -FileName "hidapi.h" -SourcePath "$env:USERPROFILE\vcpkg\installed\x64-windows\include\hidapi\hidapi.h"
$hidSucessWINH = Copy-ToCurrentDirectory -FileName "hidapi_winapi.h" -SourcePath "$env:USERPROFILE\vcpkg\installed\x64-windows\include\hidapi\hidapi_winapi.h"

if (-not $headerSuccess -or -not $dllSuccess -or -not $dllHeader -or -not $cWrapperSuccess -or -not $cWrapperImplib -or -not 
$hidSucessDLL  -or -not $hidSucessH  -or -not $hidSucessWINH) {
    Write-Host "One or more copying failed. Please check that nessecary files have copied"
    Write-Host "Single config (Ninja/GCC) build requires: libGraphiti.dll, libGraphiti.dll.a, libGraphiti_C.dll, libGraphiti_C.dll.a"
    Write-Host "If any of these are missing, rebuild the library with: .\library.ps1 -Generator `"Ninja`" -Compiler `"g++`" from the lib folder"
    exit 1
}

if (-not $dllSuccessMC -or -not $dllHeaderMC) {
    Write-Host "Multi-Config (MSVC) files not copied: Non-issue if you are building test.exe with GCC (build.ps1 default)."
    Write-Host "Only needed if you plan to link test.exe with MSVC instead of GCC. Do not mix MSVC and GCC files in this folder."
}