#!/bin/bash

# Parse command line arguments
CLEAN_BUILD=false
for arg in "$@"; do
    if [ "$arg" == "--clean" ]; then
        CLEAN_BUILD=true
    fi
done

# Detect operating system using uname
UNAME_OUTPUT=$(uname -s 2>/dev/null || echo "Unknown")

if [[ "$UNAME_OUTPUT" == "Linux" ]]; then
    OS="linux"
    BUILD_TYPE="Debug"
    EXECUTABLE_NAME="as_bundler"
    OUTPUT_NAME="pcx-bundler"
elif [[ "$UNAME_OUTPUT" == MINGW* ]] || [[ "$UNAME_OUTPUT" == MSYS* ]] || [[ "$UNAME_OUTPUT" == CYGWIN* ]] || [[ "$UNAME_OUTPUT" == "Windows_NT" ]]; then
    OS="windows"
    BUILD_TYPE="Release"
    EXECUTABLE_NAME="as_bundler.exe"
    OUTPUT_NAME="pcx-bundler.exe"
else
    echo "Unsupported operating system: $UNAME_OUTPUT"
    echo "Please report this issue with your system information."
    exit 1
fi

echo "Detected OS: $OS (uname: $UNAME_OUTPUT)"
echo "Build type: $BUILD_TYPE"

# Handle clean build
if [ "$CLEAN_BUILD" == true ]; then
    echo "Performing clean build..."
    if [ -d "build" ]; then
        echo "Removing existing build directory..."
        rm -rf build
    else
        echo "No existing build directory found."
    fi
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
    cd build
    echo "Running CMake configuration..."
    cmake ..
else
    cd build
fi

# Build the project
echo "Building project in $BUILD_TYPE mode..."
cmake --build . --config $BUILD_TYPE

# Copy executable to root folder
echo "Copying $EXECUTABLE_NAME to root as $OUTPUT_NAME..."
if [ "$OS" == "windows" ]; then
    cp $BUILD_TYPE/$EXECUTABLE_NAME ../$OUTPUT_NAME
else
    cp $EXECUTABLE_NAME ../$OUTPUT_NAME
fi

cd ..
echo "Build completed successfully!"
