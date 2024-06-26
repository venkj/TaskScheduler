#ifndef TASK_H
#define TASK_H

#include "Probe.h"
#include "MetricAggregator.h"
#include <memory>
#include <chrono>
#include <vector>

class Task
{
public:
    Task(const std::string &name, std::shared_ptr<Probe> probe, std::chrono::milliseconds interval);

    MetricAggregator &aggregator();
    std::chrono::milliseconds interval() const;
    std::chrono::time_point<std::chrono::high_resolution_clock> get_next_run_time() const;
    void update_next_run_time(std::chrono::time_point<std::chrono::high_resolution_clock> next_run_time);
    void run();

    const std::shared_ptr<Probe> &get_probe() const { return probe_; };
    const std::string &name() const { return name_; }

private:
    std::string name_;
    std::shared_ptr<Probe> probe_;
    MetricAggregator aggregator_;
    std::chrono::milliseconds interval_;
    std::chrono::time_point<std::chrono::high_resolution_clock> next_run_time_;
};

#endif // TASK_H
