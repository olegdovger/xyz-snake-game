@echo off
echo Formatting C++ code...

REM Check if clang-format is available
where clang-format >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: clang-format not found in PATH
    echo Please install clang-format or add it to your PATH
    echo Download from: https://releases.llvm.org/download.html
    pause
    exit /b 1
)

for /r src %%f in (*.cpp *.hpp *.h) do (
    echo Formatting %%f
    clang-format -i "%%f"
)

echo Code formatting complete!
