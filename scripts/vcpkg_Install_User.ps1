# Define user vcpkg path
$UserPath = "$env:USERPROFILE\vcpkg"

if (-not (Test-Path "$UserPath\vcpkg.exe")) {
    git clone https://github.com/microsoft/vcpkg.git $UserPath
    & "$UserPath\bootstrap-vcpkg.bat"
} else {
    Write-Host "vcpkg already exists, skipping clone/bootstrap."
}

# Integrate with MSBuild
& "$UserPath\vcpkg.exe" integrate install

# Install Asio
& "$UserPath\vcpkg.exe" install asio

# Install Hidapi
& "$UserPath\vcpkg.exe" install hidapi

# Add vcpkg to the user's PATH
$envPath = [Environment]::GetEnvironmentVariable("PATH", "User")
if (-not $envPath.Contains($UserPath)) {
    [Environment]::SetEnvironmentVariable("PATH", "$envPath;$UserPath", "User")
    Write-Host "`nvcpkg path added to user PATH. You may need to restart your terminal."
} else {
    Write-Host "`nvcpkg is already in user PATH."
}
