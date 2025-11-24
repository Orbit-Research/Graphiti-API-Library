param (
    [string]$Action = "buildrun"
)

# Store the original directory
$originalDir = Get-Location

# Switch to the project root (parent of the script's folder)
Set-Location (Join-Path $PSScriptRoot "..")

function clean {
    Write-Host "Running clean..."
    Remove-Item -Recurse -Force .\build\
}

function configure {
    param (
        [switch]$Debug
    )

    Write-Host "Running configure..."

    $version = @()

    if ($Debug) {
        $version += "-DCMAKE_BUILD_TYPE=Debug"
        Write-Host "-Debug enabled"
    }

    cmake -B build -S . @version
}

function build {
    param (
        [string]$Config = ""
    )

    Write-Host "Running build with changes..."

    if ($Config) {
        cmake --build build --config $Config
    } else {
        cmake --build build
    }
}

function run_ExtensionTest {
    param (
        [string]$Config = ""
    )

    Write-Host "Running current build (ExtensionTest)"

    $exePath = if ($Config) { ".\build\$Config\ExtensionTest.exe" } else { ".\build\ExtensionTest.exe" }

    if (Test-Path $exePath) {
        & $exePath
    } else {
        Write-Host "Error: $exePath not found."
    }
}

function run_Testing {
    param (
        [string]$Config = ""
    )

    Write-Host "Running current build (Testing)"

    $exePath = if ($Config) { ".\build\$Config\Testing.exe" } else { ".\build\Testing.exe" }

    if (Test-Path $exePath) {
        & $exePath
    } else {
        Write-Host "Error: $exePath not found."
    }
}

switch ($Action.ToLower()) {
    "clean" { clean }
    "configure" { configure }
    "configure_debug"{ configure -Debug }
    "build" { build }
    "build_debug" { build -Config Debug }
    "run_extension" { run_ExtensionTest }
    "run_extension_debug" { run_ExtensionTest -Config Debug }
    "run_testing" { run_Testing }
    "run_testing_debug" { run_Testing -Config Debug }
    "full_build" {
        clean
        configure
        build
    }
    "standard" {
        clean
        configure
        build
        run_Testing
    }
    "standard_debug" {
        clean
        configure -Debug
        build -Config Debug
        run_Testing -Config Debug
    }
    "buildrun" {
        build
        run_Testing
    }
    "buildrun_debug" {
        build -Config Debug
        run_Testing -Config Debug
    }
    "all" {
        clean
        configure -Debug
        build -Config Debug
        run_Testing -Config Debug
    }
    default {
        Write-Host "Error: Unknown action '$Action'."
        exit 1
    }
}

# Restore the original directory (optional)
Set-Location $originalDir