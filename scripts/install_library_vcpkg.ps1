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