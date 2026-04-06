#/*******************************************************************************
# * @file    vcpkg_Install_User.ps1
# * @company Orbit Research
# * @version v1.0
# * @date    08-12-25
# * @brief   Installs vcpkg and required packages for the project.
# *
# * @details
# * Clones and bootstraps `vcpkg` if not present, integrates with MSBuild and
# * installs required packages such as `asio` and `hidapi`. Optionally updates
# * the user's PATH to include the vcpkg location.
# *
# ******************************************************************************
# * @attention
# * Copyright (c) 2026 Orbit Research LLC
# * Distributed under GNU General Public License V3.0
# *******************************************************************************/

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
