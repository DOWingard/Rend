@echo off

set "vs_version=%~1"
set "build_type=%~2"
set "libs_type=%~3"
set "architecture=%~4"
set "hwkey_arg=%~5"

if "%architecture%"=="x86" (
    set arch="Win32"
) else if "%architecture%"=="x64" (
    set arch="x64"
) else (
    goto :InvalidArguments
)

if not "%build_type%"=="Debug" if not "%build_type%"=="Release" goto :InvalidArguments

if "%vs_version%"=="v140" (
    set generator="Visual Studio 14 2015"
) else if "%vs_version%"=="v141" (
    set generator="Visual Studio 15 2017"
) else if "%vs_version%"=="v142" (
    set generator="Visual Studio 16"
) else if "%vs_version%"=="v143" (
    set generator="Visual Studio 17"
) else (
    goto :InvalidArguments
)

if /I NOT "%libs_type%"=="staticMD" if /I NOT "%libs_type%"=="staticMT" if /I NOT "%libs_type%"=="shared" goto :InvalidArguments

set "LS_HARDWARE_KEY=OFF"
if /I "%hwkey_arg%"=="use_hardware_key" (
    set "LS_HARDWARE_KEY=ON"
) else if NOT "%hwkey_arg%"=="" (
    goto :InvalidArguments
)

if exist build (
    echo Deleting the existing build folder...
    rmdir /s /q build
)

echo.
echo Running CMake...
set "cmake_cmd=cmake -B build -G \"%generator%\" -A %arch% -DCMAKE_BUILD_TYPE=%build_type% -DLS_HARDWARE_KEY=%LS_HARDWARE_KEY%"
echo %cmake_cmd%
%cmake_cmd%

if %errorlevel% neq 0 (
    echo CMake configuration failed.
    exit /b 1
)

echo.
echo Building the project...
cmake --build build --config %build_type%

if %errorlevel% neq 0 (
    echo Build failed.
    exit /b 1
)

echo Build completed successfully.
exit /b 0

:InvalidArguments
echo Invalid arguments! Usage: build.bat [PlatformToolset] [BuildType] [Configuration] [Architecture] [use_hardware_key]
echo Supported platform toolsets: v140, v141, v142 and v143.
echo Supported build types: Debug or Release.
echo Supported configurations: staticMD, staticMT, or shared.
echo Supported architectures: x86 or x64.
exit /b 1
