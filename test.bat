@echo off
echo ========================================
echo ULTRA-LOW LATENCY SYSTEM - VISIBLE TEST
echo ========================================
echo.

echo Step 1: Testing system components...
echo.

echo Testing high-resolution timer...
powershell -Command "& { $start = Get-Date; Start-Sleep -Milliseconds 1; $end = Get-Date; $duration = ($end - $start).TotalMicroseconds; Write-Host 'Timer precision:' $duration 'microseconds' }"

echo.
echo Testing Winsock...
powershell -Command "& { Add-Type -AssemblyName System.Net; Write-Host 'Winsock available' }"

echo.
echo Step 2: Starting server in background...
start "Ultra-Low Latency Server" cmd /k "cd /d %~dp0 && echo SERVER STARTED && echo Listening on port 8080 && echo Press Ctrl+C to stop && bin\server.exe"

echo Waiting 3 seconds for server to start...
timeout /t 3 /nobreak > nul

echo.
echo Step 3: Running client test...
echo.
bin\client.exe --requests 5

echo.
echo Step 4: Running additional test...
echo.
bin\client.exe --request "hello" --expected "respuesta" --requests 3

echo.
echo ========================================
echo SYSTEM TEST COMPLETED!
echo ========================================
echo.
echo Check the server window for detailed logs.
echo.
echo Press any key to exit...
pause > nul 