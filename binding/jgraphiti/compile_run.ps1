<#
 SYNTAX
 Compile and run the Java example using the local JNI libraries.

 DESCRIPTION
 Compiles Java sources in the current directory and runs the example with
 `-Djava.library.path` set to the local directory so the JNI DLL can be found.
#>

javac *.java

java "-Djava.library.path=." -cp ..\.. binding.jgraphiti.Main