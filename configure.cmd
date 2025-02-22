@echo off
setlocal enabledelayedexpansion

:: Does the user need help?
if /I "%1" == "help" goto help
if /I "%1" == "/?" (
:help
    echo Help for configure script
    echo Syntax: path\to\source\configure.cmd [script-options] [Cmake-options]
    echo Available script-options: Codeblocks, Eclipse, Makefiles, clang, VSSolution
    echo Cmake-options: -DVARIABLE:TYPE=VALUE
    goto quit
)

:: Get the source root directory
set MICRON_SOURCE_DIR=%~dp0

:: Ensure there's no spaces in the source path
echo %MICRON_SOURCE_DIR%| find " " > NUL
if %ERRORLEVEL% == 0 (
    echo. && echo   Your source path contains at least one space.
    echo   This will cause problems with building.
    echo   Please rename your folders so there are no spaces in the source path,
    echo   or move your source to a different folder.
    goto quit
)

echo   Checking build tool ...
:: Detect presence of cmake
cmd /c cmake --version 2>&1 | find "cmake version" > NUL || goto cmake_notfound

:: Detect presence of ninja build
cmd /c ninja --version >nul 2>&1 || goto ninja_notfound

:: Set default generator
set CMAKE_GENERATOR="Ninja"
set MICRON_ARCH=
set TOOCHAIN_FILE=

:: Target archiver
echo  Target CPU architecture to build:
echo  1. Arm32(armv7a)
echo  2. Riscv32
echo  3. Exit
set  /p choose=Target option: 

if "%choose%"=="1" (
    set MICRON_ARCH="arm32"
) else if "%choose%"=="2" (
    set MICRON_ARCH="riscv32"
) else if "%choose%"=="3" (
    echo Exit script.
    goto quit
) else (
    echo Invalid option. Please choose 1, 2, or 3.
)

:: Checkpoint
if not defined MICRON_ARCH (
    echo Unknown build architecture
    goto quit
)

:: Set compiler
if "%choose%"=="1" (
    set MICRON_ARCH="arm32"
    set TOOCHAIN_FILE=%MICRON_SOURCE_DIR%/toolchain.cmake
) else if "%choose%"=="2" (
    set MICRON_ARCH="riscv32"
) else if "%choose%"=="3" (
    echo Exit script.
    goto quit
) else (
    echo Invalid option. Please choose 1, 2, or 3.
)

:: Parse command line parameters
set CMAKE_PARAMS=
set BUILD_TOOLS_FLAG=

:: Inform the user about the default build
if "!CMAKE_GENERATOR!" == "Ninja" (
    echo This script defaults to Ninja. Type "configure help" for alternative options.
)

:: Create directories
set MICRON_BUILD_PATH=build-%MICRON_ARCH%

if "%MICRON_SOURCE_DIR%" == "%CD%\" (
    set CD_SAME_AS_SOURCE=1
    echo Creating directories in %MICRON_BUILD_PATH%

    if not exist %MICRON_BUILD_PATH% (
        mkdir %MICRON_BUILD_PATH%
    )
    cd %MICRON_BUILD_PATH%
)

echo Preparing MICRON...

if EXIST CMakeCache.txt (
    del CMakeCache.txt /q
)

:: run cmake
cmake -G %CMAKE_GENERATOR% -DENABLE_CCACHE:BOOL=OFF -DCMAKE_TOOLCHAIN_FILE:FILEPATH=%TOOCHAIN_FILE% -DARCH:STRING=%MICRON_ARCH% %BUILD_TOOLS_FLAG% %CMAKE_PARAMS% "%MICRON_SOURCE_DIR%"


:: If Error
if %ERRORLEVEL% NEQ 0 (
    goto quit
)

if "%CD_SAME_AS_SOURCE%" == "1" (
    set ENDV= from %MICRON_BUILD_PATH%
)

set ENDV= Execute appropriate build commands ^(ex: ninja, make, nmake, etc...^)%ENDV%

echo. && echo Configure script complete^^!%ENDV%

goto quit

:cmake_notfound
echo Unable to find cmake, if it is installed, check your PATH variable.

:ninja_notfound
echo Error ninja build tool not found.
echo run command "choco install ninja" and set path global enviroment

:quit
endlocal
exit /b
