@echo off
echo Building and running PNG image analysis...
echo.

REM Create build directory if it doesn't exist
if not exist build mkdir build
cd build

REM Configure with CMake
cmake .. -G "MinGW Makefiles"
if errorlevel 1 (
    echo Error: CMake configuration failed
    pause
    exit /b 1
)

REM Build the project
cmake --build .
if errorlevel 1 (
    echo Error: Build failed
    pause
    exit /b 1
)

REM Run the analyzer
echo.
echo Running image analysis...
analyze_resources.exe

echo.
echo Analysis complete! Check resource_details.txt for results.
pause
