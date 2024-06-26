#include "TaskScheduler.h"
#include "NetworkProbe.h"
#include "SystemProbe.h"
#include <iostream>
#include <thread>

int main()
{
    auto network_probe_google = std::make_shared<NetworkProbe>("google.com");
    auto system_probe = std::make_shared<SystemProbe>();

    // Creating TaskScheduler
    TaskScheduler scheduler;
    scheduler.add_task("Network monitoring: google.com", network_probe_google, std::chrono::milliseconds(5000)); // 5 seconds interval
    scheduler.add_task("System monitoring", system_probe, std::chrono::milliseconds(1000));
    // Starting scheduler
    scheduler.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    scheduler.stop();

    std::cout << "Scheduler stopped. Waiting for 5 seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Resuming scheduler
    std::cout << "Resuming scheduler..." << std::endl;
    scheduler.resume();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    scheduler.stop();

    std::cout << "Scheduler stopped again. Waiting for 5 seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Resuming scheduler again
    std::cout << "Resuming scheduler again..." << std::endl;
    scheduler.resume();
    std::this_thread::sleep_for(std::chrono::seconds(40));
    scheduler.stop();
    std::cout << "Stopping" << std::endl;

    // Printing aggregate metrics
    scheduler.print_aggregates();

    return 0;
}