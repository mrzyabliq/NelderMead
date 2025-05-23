@echo off
setlocal

:: 1. Configuration selection
echo Select your build configuration:
echo 1 - Release (default)
echo 2 - Debug
set /p choice="Enter number [1-2]: "

if "%choice%"=="2" (
    set CONFIG=Debug
) else (
    set CONFIG=Release
)

cd .\build\bin\%CONFIG%
NelderMeadTests.exe

endlocal