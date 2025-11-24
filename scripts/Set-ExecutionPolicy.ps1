# Check if running as admin; if not, relaunch as admin
if (-NOT ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Start-Process powershell.exe "-NoProfile -ExecutionPolicy Bypass -File `"$($MyInvocation.MyCommand.Path)`"" -Verb RunAs
    exit
}

# Set ExecutionPolicy (only runs if admin)
Set-ExecutionPolicy RemoteSigned -Scope CurrentUser -Force

# Optional: Show confirmation
Write-Host "Execution Policy set to RemoteSigned!" -ForegroundColor Green