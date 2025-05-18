@echo off
setlocal

:: 1. Configuration selection
echo Select build configuration:
echo 1 - Release (default)
echo 2 - Debug
set /p choice="Enter number [1-2]: "

if "%choice%"=="2" (
    set CONFIG=Debug
) else (
    set CONFIG=Release
)

:: 2. Clean previous build
if exist "build\" rmdir /s /q build

:: 3. Build project
mkdir build
cd build
cmake ..
cmake --build . --config %CONFIG%

:: 4. Run application
if exist "bin\%CONFIG%\NelderMeadFrontend.exe" (
    cd bin\%CONFIG%
    start NelderMeadFrontend.exe
) else (
    echo Error: Could not find executable file
)

endlocal