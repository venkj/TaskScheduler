# Task Scheduler

## Overview
This project is a Task Scheduler that supports network and system probes. It collects various metrics and calculates aggregate metrics like average, minimum, and maximum. The scheduler supports customizable execution frequencies for each task and handles failures gracefully.

## Features
- Network Probes:
  - DNS Resolve Time
  - TCP Connect Time
  - ICMP Ping Time
- System Probes:
  - Total Swapfile Size and Current Usage
  - Current Physical Memory Used by the Process
  - Total Packets Sent and Received on a Specific Network Interface
- Aggregates metrics (average, minimum, maximum)
- Handles task failures gracefully

## Prerequisites
- GCC 14.1.0
- CMake 3.10 or higher
- Boost 1.85
- Google Test

## Directory Structure
```
TaskScheduler/
├── include/
│   ├── Metric.h
│   ├── MetricAggregator.h
│   ├── NetworkProbe.h
│   ├── Probe.h
│   ├── SystemProbe.h
│   └── Task.h
│   └── TaskScheduler.h
├── src/
│   ├── MetricAggregator.cpp
│   ├── NetworkProbe.cpp
│   ├── SystemProbe.cpp
│   ├── TaskScheduler.cpp
│   ├── Task.cpp
├── tests/
│   ├── main.cpp
│   ├── MetricAggregatorTest.cpp
│   ├── NetworkProbeTest.cpp
│   ├── SystemProbeTest.cpp
│   ├── TaskSchedulerTest.cpp
│   └── CMakeLists.txt
├── CMakeLists.txt
└── build.sh
└── main.cpp
└── README.md

```

## Build and Test Instructions

### Using `build.sh` Script

1. Ensure the `build.sh` script has execute permissions:
    ```sh
    chmod +x build.sh
    ```

2. Run the `build.sh` script:
    ```sh
    ./build.sh
    ```

### Manual Build and Test

1. **Clone the repository**:
    ```sh
    git clone https://github.com/venkj/TaskScheduler.git
    cd TaskScheduler
    ```

2. **Initialize Google Test submodule**:
    ```sh
    git submodule update --init --recursive
    ```

3. **Create a build directory**:
    ```sh
    mkdir build
    cd build
    ```

4. **Run CMake**:
    ```sh
    CC=gcc-14 CXX=g++-14 cmake ..
    ```

5. **Build the project**:
    ```sh
    make
    ```

6. **Run the tests**:
    ```sh
    cd tests
    ctest --output-on-failure
    ```

## Environment
This project was built and tested on macOS Monterey.

## Acknowledgments
- Google Test: [https://github.com/google/googletest](https://github.com/google/googletest)
- Boost: [https://www.boost.org/](https://www.boost.org/)

### Note
If you encounter any issues during the build process, ensure that the paths and environment variables are correctly set, and that the required versions of GCC, CMake, Boost, and Google Test are installed.