@echo off
@setlocal enabledelayedexpansion
pushd %~dp0

rem Get parameters
:PARSE
set arg=%1
if defined arg (
	if "%arg%"=="/?" (
		goto :HELP
		exit /b
	)
	if "%arg%"=="/Q" (
		set qtdir=%2
		shift
	)
	if "%arg%"=="/G" (
		set qtgen=%2
		shift
	)
	if "%arg%"=="/A" (
		set qtarch=%2
		shift
	)
	shift
	goto :PARSE
)

rem Get Qt location
if not defined qtdir (
	rem Check if environment variable is set
	if defined QT_PATH (
		set default_qtdir=%QT_PATH%
		goto :ASK_QTDIR
	)

	rem Try to find default Qt location
	set default_qtdir=C:\Qt
	if not exist !default_qtdir! goto :ASK_QTDIR
	set qt_ver=
	for /F %%i in ('dir !default_qtdir! /B /AD ^| findstr /R "^[0-9]*.[0-9]*.[0-9]*$"') do if not defined qt_ver set qt_ver=%%i
	set default_qtdir=!default_qtdir!\!qt_ver!

	:ASK_QTDIR
	set /p "qtdir=Enter Qt install directory (default: !default_qtdir!): "
	if not defined qtdir set qtdir=!default_qtdir!
)

rem Get Qt generator
if not defined qtgen (
	set "default_qtgen=Visual Studio 17 2022"
	set /p "qtgen=Enter Qt generator (default: !default_qtgen!): "
	if not defined qtgen set qtgen=!default_qtgen!
)

rem Get target architecture
if not defined qtarch (
	set "default_qtarch=64"
	set /p "qtarch=Enter target architecture (32 or 64, default: !default_qtarch!): "
	if not defined qtarch set qtarch=!default_qtarch!
)

rem Validate parameters
if not exist !qtdir! (
	echo Invalid Qt directory ^(!qtdir!^)
	goto :END
)
if not "!qtarch!"=="64" ( 
	if not "!qtarch!"=="32" (
		echo Invalid target architecture ^(!qtarch!^)
		goto :END
	)
)
where /q cmake
if errorlevel 1 (
	echo Could not find CMake on PATH
	goto :END
)

rem Run CMake
set QT_PATH=!qtdir!
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S./ -B./build -G "!qtgen!" -T host=x64 -A x!qtarch!
:END
popd
exit /b

rem Print help message
:HELP
echo.
echo GenerateSolution.bat [/?] [/Q dir] [/G generator] [/A arch]
echo.
echo Description:
echo    Generate a Visual Studio solution for the project using CMake.
echo    If a parameter is not given, the script will ask the user for
echo    the value.
echo.
echo Parameter List:
echo    /Q    Specify Qt install directory. (defaults first to QT_PATH
echo          environment variable, then to C:\Qt\^<latest^>)
echo.
echo    /G    Generator string passed to CMake. (defaults to "Visual
echo          Studio 17 2022")
echo.
echo    /A    Target architecture. Valid values are 32 or 64. 
echo          (defaults to 64)
echo.
echo.   /?    Displays this help message.
exit /b