@echo off
cd /d "%~dp0"

set "PATH=C:\msys64\ucrt64\bin;%PATH%"

start "JP-Bike Rebalance - servidor" cmd /k ".\servidor.exe 8080"

timeout /t 1 /nobreak >nul
start http://127.0.0.1:8080
