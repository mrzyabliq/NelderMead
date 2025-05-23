@echo off
setlocal enabledelayedexpansion

:: 1. Configuration selection
:config_select
echo Select build configuration:
echo 1 - Release (default)
echo 2 - Debug
set /p choice="Enter number [1-2]: "
if not defined choice set choice=1

if "%choice%"=="2" (
    set CONFIG=Debug
) else (
    if not "%choice%"=="1" (
        echo Invalid choice, please try again
        goto config_select
    )
    set CONFIG=Release
)

:: 2. Clean previous build
echo Cleaning previous build...
if exist "build" (
    rmdir /s /q build
    if errorlevel 1 (
        echo Error: Failed to clean build directory
        pause
        exit /b 1
    )
)

:: 3. Build project
echo Creating build directory...
mkdir build
if errorlevel 1 (
    echo Error: Failed to create build directory
    pause
    exit /b 1
)

cd build

echo Configuring project with CMake...
cmake .. -DCMAKE_BUILD_TYPE=%CONFIG%
if errorlevel 1 (
    echo Error: CMake configuration failed
    pause
    exit /b 1
)

echo Building project (%CONFIG%)...
cmake --build . --config %CONFIG% -- /m
if errorlevel 1 (
    echo Error: Build failed
    pause
    exit /b 1
)

:: 4. Run application
echo Looking for executable...
set EXE_PATH=bin\%CONFIG%\NelderMeadFrontend.exe

if exist "%EXE_PATH%" (
    echo Starting application...
    .\%EXE_PATH%
) else (
    echo Error: Could not find executable file at "%EXE_PATH%"
    echo Available files in bin\%CONFIG%\:
    dir /b bin\%CONFIG%\
    pause
    exit /b 1
)

endlocal