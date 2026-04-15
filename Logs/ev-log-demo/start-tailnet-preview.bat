@echo off
setlocal
cd /d "%~dp0"

echo Starting EV Log Copilot on 0.0.0.0:3000 ...
start "EV Log Copilot Tailnet" /min python -m uvicorn server.app:app --host 0.0.0.0 --port 3000 1>uvicorn.out.log 2>uvicorn.err.log

echo.
echo Tailnet preview should be available on:
echo   http://100.114.70.30:3000/
echo   http://100.114.70.30:3000/sessions
echo   http://100.114.70.30:3000/settings
echo.
echo If Windows Firewall blocks access, run this file once as Administrator after creating the rule manually.
