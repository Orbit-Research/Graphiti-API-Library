#/*******************************************************************************
# * @file    Set-ExecutionPolicy.ps1
# * @company Orbit Research
# * @version v1.0
# * @date    08-12-25
# * @brief   Sets PowerShell execution policy for the current user.
# *
# * @details
# * Checks for administrative privileges and relaunches as admin if necessary,
# * then sets the execution policy to `RemoteSigned` for the current user to
# * allow local script execution during development.
# *
# ******************************************************************************
# * @attention
# * Copyright (c) 2026 Orbit Research LLC
# * Distributed under GNU General Public License V3.0
# *******************************************************************************/

# Check if running as admin; if not, relaunch as admin
if (-NOT ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Start-Process powershell.exe "-NoProfile -ExecutionPolicy Bypass -File `"$($MyInvocation.MyCommand.Path)`"" -Verb RunAs
    exit
}

# Set ExecutionPolicy (only runs if admin)
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser -Force

# Optional: Show confirmation
Write-Host "Execution Policy set to RemoteSigned!" -ForegroundColor Green