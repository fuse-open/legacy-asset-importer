@echo off
pushd "%~dp0"

if not defined VSINSTALLDIR (
	call "%VS120COMNTOOLS%\vsvars32.bat"
)

rem Generate and build (x86)
mkdir Native\build\vs2013\x86
pushd Native\build\vs2013\x86
cmake -G"Visual Studio 12" "%~dp0Native" || goto ERROR
msbuild uAsset.sln /p:Configuration=Debug /m || goto ERROR
msbuild uAsset.sln /p:Configuration=Release /m || goto ERROR
popd

rem Generate and build (x64)
mkdir Native\build\vs2013\x64
pushd Native\build\vs2013\x64
cmake -G"Visual Studio 12 Win64" "%~dp0Native" || goto ERROR
msbuild uAsset.sln /p:Configuration=Debug /m || goto ERROR
msbuild uAsset.sln /p:Configuration=Release /m || goto ERROR
popd

rem Build .NET assemblies
msbuild Uno.Native.sln /p:Configuration=Release || goto ERROR

rem Done
popd && exit /b 0

:ERROR
echo BUILD FAILED.
popd && exit /b 1
