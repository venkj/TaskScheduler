#include "Task.h"
#include <iostream>

Task::Task(const std::string &name, std::shared_ptr<Probe> probe, std::chrono::milliseconds interval)
    : name_(name),
      probe_(std::move(probe)),
      aggregator_(),
      interval_(interval),
      next_run_time_(std::chrono::high_resolution_clock::now() + interval)
{
}

MetricAggregator &Task::aggregator()
{
    return aggregator_;
}

std::chrono::milliseconds Task::interval() const
{
    return interval_;
}

std::chrono::time_point<std::chrono::high_resolution_clock> Task::get_next_run_time() const
{
    return next_run_time_;
}

void Task::update_next_run_time(std::chrono::time_point<std::chrono::high_resolution_clock> next_run_time)
{
    next_run_time_ = next_run_time;
}

void Task::run()
{
    try
    {
        auto metrics = probe_->collect_metrics();
        for (const auto &metric : metrics)
        {
            aggregator_.add_metric(metric);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Task: " << name_ << " failed: " << e.what() << std::endl;
    }
    next_run_time_ = std::chrono::high_resolution_clock::now() + interval_;
}