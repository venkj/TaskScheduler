cmake_minimum_required(VERSION 3.10)
project(TaskScheduler)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler to GCC 14.1.0
set(CMAKE_C_COMPILER "/usr/local/bin/gcc-14")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-14")

# Find Boost
find_package(Boost 1.85 REQUIRED COMPONENTS system)

# Include directories
include_directories(include ${Boost_INCLUDE_DIRS})

# Source files
file(GLOB SOURCES "src/*.cpp")

# Create build directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)

# Add visibility flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fvisibility=hidden -fvisibility-inlines-hidden)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
endif()

# Add a library target for the project sources
add_library(task_scheduler_lib ${SOURCES})

# Add main executable
add_executable(task_scheduler main.cpp)
target_link_libraries(task_scheduler task_scheduler_lib ${Boost_LIBRARIES})

# Add a custom target for cleaning the build directory
add_custom_target(clean-build
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/build
    COMMENT "Cleaning build directory"
)

# Enable testing
enable_testing()

# Add tests subdirectory
add_subdirectory(tests)
