function Copy-ToCurrentDirectory {
    param (
        [string]$FileName,
        [string]$SourcePath
    )

    Write-Host "Deleting existing $FileName (if any)..."
    Remove-Item $FileName -ErrorAction SilentlyContinue

    if (!(Test-Path $SourcePath)) {
        Write-Error "$FileName not found at: $SourcePath"
        return $false
    }

    Copy-Item -Path $SourcePath -Destination $FileName -Force
    Write-Host "Copied $FileName to current directory."
    return $true
}

# Copy DLL
$dllSuccess = Copy-ToCurrentDirectory -FileName "libGraphiti.dll" -SourcePath "$env:USERPROFILE\graphiti\bin\libGraphiti.dll"
$dllHeader = Copy-ToCurrentDirectory -FileName "libGraphiti.dll.a" -SourcePath "$env:USERPROFILE\graphiti\lib\libGraphiti.dll.a"

# Copy Mult-Config DLL
$dllSuccessMC = Copy-ToCurrentDirectory -FileName "Graphiti_C.dll" -SourcePath "$env:USERPROFILE\graphiti\bin\Graphiti_C.dll"
$dllHeaderMC = Copy-ToCurrentDirectory -FileName "Graphiti_C.lib" -SourcePath "$env:USERPROFILE\graphiti\lib\Graphiti_C.lib"

# Copy hidapi
$hidSucessDLL = Copy-ToCurrentDirectory -FileName "hidapi.dll" -SourcePath "$env:USERPROFILE\vcpkg\installed\x64-windows\bin\hidapi.dll"
$hidSucessH = Copy-ToCurrentDirectory -FileName "hidapi.h" -SourcePath "$env:USERPROFILE\vcpkg\installed\x64-windows\include\hidapi\hidapi.h"
$hidSucessWINH = Copy-ToCurrentDirectory -FileName "hidapi_winapi.h" -SourcePath "$env:USERPROFILE\vcpkg\installed\x64-windows\include\hidapi\hidapi_winapi.h"

if (-not $dllSuccess -or -not $dllHeader -or -not 
$hidSucessDLL  -or -not $hidSucessH  -or -not $hidSucessWINH) {
    Write-Host "One or more copying failed. Please check that nessecary files have copied"
    Write-Host "Single config creates libGraphiti.dll and libGraphiti.dll.a"
    Write-Host "One or the other is fine"
    exit 1
}

if (-not $dllSuccessMC -or -not $dllHeaderMC) {
    Write-Host "Multi-Config files not copied: Non-issue if using Single-Config"
}