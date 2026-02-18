@echo off
setlocal enabledelayedexpansion

:: Parse command line arguments
set CLEAN_BUILD=0
:parse_args
if "%~1"=="" goto end_parse
if /i "%~1"=="--clean" set CLEAN_BUILD=1
shift
goto parse_args
:end_parse

set BUILD_TYPE=Release
set EXECUTABLE_NAME=as_bundler.exe
set OUTPUT_NAME=pcx-bundler.exe

echo Detected OS: Windows
echo Build type: %BUILD_TYPE%

:: Handle clean build
if %CLEAN_BUILD%==1 (
    echo Performing clean build...
    if exist "build" (
        echo Removing existing build directory...
        rmdir /s /q build
    ) else (
        echo No existing build directory found.
    )
)

:: Create build directory if it doesn't exist
if not exist "build" (
    echo Creating build directory...
    mkdir build
    cd build
    echo Running CMake configuration...
    cmake ..
    if errorlevel 1 (
        echo CMake configuration failed!
        cd ..
        exit /b 1
    )
) else (
    cd build
)

:: Build the project
echo Building project in %BUILD_TYPE% mode...
cmake --build . --config %BUILD_TYPE%
if errorlevel 1 (
    echo Build failed!
    cd ..
    exit /b 1
)

:: Copy executable to root folder
echo Copying %EXECUTABLE_NAME% to root as %OUTPUT_NAME%...
if exist "%BUILD_TYPE%\%EXECUTABLE_NAME%" (
    copy /y "%BUILD_TYPE%\%EXECUTABLE_NAME%" "..\%OUTPUT_NAME%" > nul
    if errorlevel 1 (
        echo Failed to copy executable!
        cd ..
        exit /b 1
    )
) else (
    echo Error: Executable not found at %BUILD_TYPE%\%EXECUTABLE_NAME%
    cd ..
    exit /b 1
)

cd ..
echo Build completed successfully!