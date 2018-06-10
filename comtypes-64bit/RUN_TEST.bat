@echo off
echo This script MUST be run with Administrator privileges

:: Switch to current directory
cd /d "%~dp0"

:: Configure Visual Studio 2017 command prompt
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\Tools\VsDevCmd.bat"

echo Generating TLB file...
midl /env x64 PyComServer.idl


echo Runing test script...
:: Script fails with 64bit Python 3
set PYTHON_DIR=C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python36_64
::set PYTHON_DIR=C:\Program Files (x86)\Microsoft Visual Studio\Shared\Python36_86
"%PYTHON_DIR%\python.exe" PyComServer.py

pause
