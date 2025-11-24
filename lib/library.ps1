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

#Add graphiti to PATH
$graphitiPath = "$env:USERPROFILE\graphiti"
$envPath = [Environment]::GetEnvironmentVariable("PATH", "User")

if (-not $envPath.Contains($graphitiPath)) {
    [Environment]::SetEnvironmentVariable("PATH", "$envPath;$graphitiPath", "User")
    Write-Host "`nGraphiti path added to user PATH. You may need to restart your terminal."
} else {
    Write-Host "`nGraphiti is already in user PATH."
}