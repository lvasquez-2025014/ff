@echo off
title Instalador de dependencias DirectX para BlueStacks
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo Solicitando permisos de Administrador...
    powershell -Command "Start-Process '%~f0' -Verb RunAs"
    exit /b
)

cd /d "%~dp0"

echo ========================================================
echo Copiando dependencias DirectX a System32 y BlueStacks...
echo ========================================================

copy /y "d3dx11_43.dll" "C:\Windows\System32\d3dx11_43.dll"
copy /y "D3DCompiler_43.dll" "C:\Windows\System32\D3DCompiler_43.dll"

if exist "C:\Program Files\BlueStacks_nxt" (
    copy /y "d3dx11_43.dll" "C:\Program Files\BlueStacks_nxt\d3dx11_43.dll"
    copy /y "D3DCompiler_43.dll" "C:\Program Files\BlueStacks_nxt\D3DCompiler_43.dll"
)

echo.
echo ========================================================
echo   Librerias instaladas correctamente!
echo   Ya puedes inyectar con Extreme Injector en HD-Player.exe.
echo ========================================================
pause
