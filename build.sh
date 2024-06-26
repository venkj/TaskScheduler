#!/bin/bash

# Directory variables
BUILD_DIR="build"
SOURCE_DIR=$(pwd)

# Function to clean build directory
clean_build_dir() {
    if [ -d "$BUILD_DIR" ]; then
        echo "Cleaning build directory..."
        rm -rf "$BUILD_DIR"
    fi
}

# Function to create build directory
create_build_dir() {
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
}

# Function to run cmake and make with GCC 14.1.0
run_cmake_and_make() {
    echo "Running CMake with GCC 14.1.0..."
    cd "$BUILD_DIR"
    CC=gcc-14 CXX=g++-14 cmake "$SOURCE_DIR"
    if [ $? -ne 0 ]; then
        echo "CMake configuration failed"
        exit 1
    fi

    echo "Running make..."
    make
    if [ $? -ne 0 ]; then
        echo "Build failed"
        exit 1
    fi
}

# Function to run tests if make is successful
run_tests() {
    echo "Running tests..."
    ctest --output-on-failure
    if [ $? -ne 0 ]; then
        echo "Tests failed"
        exit 1
    fi
}

# Main script
clean_build_dir
create_build_dir
run_cmake_and_make
run_tests

echo "Build and tests successful!"
