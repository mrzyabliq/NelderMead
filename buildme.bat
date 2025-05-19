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
if errorlevel 1 (
    echo Error: Failed to clean build directory
    pause
    exit /b 1
)

:: 3. Build project
mkdir build
if errorlevel 1 (
    echo Error: Failed to create build directory
    pause
    exit /b 1
)

cd build
cmake ..
if errorlevel 1 (
    echo Error: CMake configuration failed
    pause
    exit /b 1
)

cmake --build . --config %CONFIG%
if errorlevel 1 (
    echo Error: Build failed
    pause
    exit /b 1
)

:: 4. Run application
if exist "bin\%CONFIG%\NelderMeadFrontend.exe" (
    cd bin\%CONFIG%
    start NelderMeadFrontend.exe
) else (
    echo Error: Could not find executable file
    pause
    exit /b 1
)

endlocal