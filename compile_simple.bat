@echo off
echo ========================================
echo Simple Ultra-Low Latency System Compiler
echo ========================================
echo.

REM Set MinGW path
set MINGW_PATH=C:\msys64\mingw64\bin
set PATH=%MINGW_PATH%;%PATH%

REM Check if g++ is available
g++ --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: g++ not found. Please install MinGW-w64.
    pause
    exit /b 1
)

echo Compiling Ultra-Low Latency System...
echo.

REM Create output directory
if not exist bin mkdir bin

REM Compile server
echo Compiling server...
g++ -std=c++17 -O2 -march=native -mtune=native -DNDEBUG ^
    src/server.cpp ^
    -o bin/server.exe ^
    -lws2_32

if errorlevel 1 (
    echo ERROR: Failed to compile server
    pause
    exit /b 1
)

REM Compile client
echo Compiling client...
g++ -std=c++17 -O2 -march=native -mtune=native -DNDEBUG ^
    src/client.cpp ^
    -o bin/client.exe ^
    -lws2_32

if errorlevel 1 (
    echo ERROR: Failed to compile simple client
    pause
    exit /b 1
)

echo.
echo ========================================
echo Compilation completed successfully!
echo ========================================
echo.
echo Executables created in: bin\
echo.
echo Available executables:
echo   - server.exe          (Ultra-latency server)
echo   - client.exe          (Latency testing client)
echo.
echo To run the system:
echo   1. Start server:     bin\server.exe
echo   2. Test with client: bin\client.exe --requests 5
echo   3. Run full test:    test_visible.bat
echo.
echo Press any key to continue...
pause >nul 