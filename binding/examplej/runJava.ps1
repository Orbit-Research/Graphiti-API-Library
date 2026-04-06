# Runs .java files in the current directory with the library

# Save current location (binding/examplej)
$currentDir = Get-Location

# Move two levels up to project root (Graphiti_API_Library_0_1)
Set-Location ..\..

# Step 1: Compile only .java files in binding/examplej/
$javaSourceDir = "binding/examplej"
$javaFiles = Get-ChildItem -Path $javaSourceDir -Recurse -Filter *.java | ForEach-Object { $_.FullName }
javac $javaFiles

# Step 2: Define the base java command
$javaCommand = 'java "-Djava.library.path=binding/jgraphiti" -cp .'

# Step 3: Get all .class files under binding/examplej
$classFiles = Get-ChildItem -Path $javaSourceDir -Filter "*.class" -Recurse

# Step 4: Convert file paths to fully-qualified class names
$arguments = $classFiles | ForEach-Object {
    $relativePath = $_.FullName.Replace((Get-Location).Path + [System.IO.Path]::DirectorySeparatorChar, "")
    $classNameWithPath = [System.IO.Path]::ChangeExtension($relativePath, $null)
    ($classNameWithPath -replace "[/\\]", ".").TrimEnd(".")
}

# Step 5: Join class names into single string
$argString = $arguments -join " "

# Step 6: Build and run the full java command
$fullCommand = "$javaCommand $argString"
Write-Host "Running: $fullCommand"
Invoke-Expression $fullCommand

# Step 7: Return to the original folder (binding/examplej)
Set-Location $currentDir
