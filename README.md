# Task Scheduler

## Overview
This project is a Task Scheduler that supports network and system probes. It collects various metrics and calculates aggregate metrics like average, minimum, maximum and count. The scheduler supports customizable execution frequencies for each task and handles failures gracefully.

## Features
- Network Probes:
  - DNS Resolve Time
  - TCP Connect Time
- System Probes:
  - Total Swapfile Size
  - Current Physical Memory Used by the Process
  - Total Packets Sent and Received on Network Interface
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

3. Run the Application:
    ```sh
    ./build/task_scheduler
    ```

## Environment
This project was built and tested on macOS Monterey.

## Acknowledgments
- Google Test: [https://github.com/google/googletest](https://github.com/google/googletest)
- Boost: [https://www.boost.org/](https://www.boost.org/)

### Note
If you encounter any issues during the build process, ensure that the paths and environment variables are correctly set, and that the required versions of GCC, CMake, Boost, and Google Test are installed.
