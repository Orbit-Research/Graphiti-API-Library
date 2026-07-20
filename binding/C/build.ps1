<#
 SYNTAX
 Build example C client linking against `Graphiti_C` library.

 DESCRIPTION
 Compiles `main.c` into `test.exe` linking to the local `Graphiti_C` library.
#>

Write-Host "Building Graphiti C example..."
Write-Host "Compiling main.c to test.exe..."

gcc main.c -o test.exe -L. -lGraphiti_C

if ($LASTEXITCODE -eq 0) {
    Write-Host "Build successful! Created test.exe"
} else {
    Write-Host "Build failed with exit code $LASTEXITCODE"
}