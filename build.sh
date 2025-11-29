#!/bin/bash

# MDV-Qt Build Script
# This script builds the MDV-Qt markdown viewer application

set -e  # Exit on any error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR"

BUILD_DIR="$PROJECT_DIR/build"

echo "MDV-Qt Build Script"
echo "=================="

# Check if Qt6 is installed
if ! command -v qmake6 &> /dev/null && ! command -v qmake-qt6 &> /dev/null; then
    echo "Qt6 development tools not found!"
    echo "Please install Qt6 with: sudo pacman -S qt6-base qt6-tools qt6-webengine"
    exit 1
fi

# Check if cmake is available
if ! command -v cmake &> /dev/null; then
    echo "CMake not found!"
    exit 1
fi

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

echo "Cleaning previous build..."
cd "$BUILD_DIR"
rm -rf ./*

echo "Configuring project..."
cmake .. -DCMAKE_PREFIX_PATH=/usr/lib/qt6

echo "Building project..."
cmake --build . --parallel

echo ""
echo "Build completed successfully!"
echo "You can run the application with: ./mdviewer"
echo "from the $BUILD_DIR directory"