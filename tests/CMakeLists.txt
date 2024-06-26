cmake_minimum_required(VERSION 3.10)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

# Find Boost
find_package(Boost 1.85 REQUIRED COMPONENTS system)

# Test source files
file(GLOB TEST_SOURCES "*.cpp")

# Add test executable
add_executable(test_scheduler ${TEST_SOURCES})

# Add visibility flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(test_scheduler PRIVATE -fvisibility=hidden -fvisibility-inlines-hidden)
    set_target_properties(test_scheduler PROPERTIES CXX_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN YES)
endif()

# Link the test executable against GoogleTest, Boost, and the project's library
target_link_libraries(test_scheduler gtest_main gmock task_scheduler_lib ${Boost_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(test_scheduler)
