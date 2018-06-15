@echo off

rem Load VS2013 environment
if not defined VSINSTALLDIR call "%VS120COMNTOOLS%\vsvars32.bat"

rem Generate x86 solution
mkdir "%~dp0\build\vs2013\x86"
cd "%~dp0\build\vs2013\x86"
cmake -G"Visual Studio 12" "%~dp0"
cd "%~dp0"

rem Generate x64 solution
mkdir "%~dp0\build\vs2013\x64"
cd "%~dp0\build\vs2013\x64"
cmake -G"Visual Studio 12 Win64" "%~dp0"
cd "%~dp0"

rem Build x86 Debug binaries
msbuild "%~dp0\build\vs2013\x86\Xli.sln" /p:Configuration=Debug /m
if %errorlevel% neq 0 exit /b %errorlevel%

rem Build x86 Release binaries
msbuild "%~dp0\build\vs2013\x86\Xli.sln" /p:Configuration=Release /m
if %errorlevel% neq 0 exit /b %errorlevel%

rem Build x64 Debug binaries
msbuild "%~dp0\build\vs2013\x64\Xli.sln" /p:Configuration=Debug /m
if %errorlevel% neq 0 exit /b %errorlevel%

rem Build x64 Release binaries
msbuild "%~dp0\build\vs2013\x64\Xli.sln" /p:Configuration=Release /m
if %errorlevel% neq 0 exit /b %errorlevel%
