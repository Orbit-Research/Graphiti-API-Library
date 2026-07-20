#/*******************************************************************************
# * @file    install_library_vcpkg.ps1
# * @company Orbit Research
# * @version v1.0
# * @date    08-12-25
# * @brief   Installs library dependencies and builds the library using vcpkg.
# *
# * @details
# * Calls the vcpkg installer script, switches to the project root and invokes
# * the library build script under `lib/`. Uses literal paths to be robust with
# * spaces in paths and provides helpful debug output.
# *
# ******************************************************************************
# * @attention
# * Copyright (c) 2026 Orbit Research LLC
# * Distributed under GNU General Public License V3.0
# *******************************************************************************/

# Debug output (optional)
Write-Host "Script directory: $PSScriptRoot"

# Installs Library dependencies - SAFE CALL
& "$PSScriptRoot\vcpkg_Install_User.ps1"

# Store the original directory
$originalDir = Get-Location

# Switch to the project root (parent of the script's folder)
$projectRoot = (Get-Item $PSScriptRoot).Parent.FullName
Set-Location -LiteralPath $projectRoot

# Debug output (optional)
Write-Host "Now in project root: $(Get-Location)"

### Builds and installs library ###

# Enter lib folder using LITERAL path to handle spaces
$libPath = Join-Path -Path $projectRoot -ChildPath "lib"
if (-not (Test-Path -LiteralPath $libPath)) {
    throw "Lib folder not found at: $libPath"
}

Push-Location -LiteralPath $libPath

# Execute library script using full path
$libraryScript = Join-Path -Path $libPath -ChildPath "library.ps1"
if (-not (Test-Path -LiteralPath $libraryScript)) {
    throw "library.ps1 not found at: $libraryScript"
}

& $libraryScript

# Exit lib folder
Pop-Location

# Restore the original directory
Set-Location -LiteralPath $originalDir

# Keeps window open until user presses Enter to view execution
Read-Host -Prompt "Press Enter to exit"