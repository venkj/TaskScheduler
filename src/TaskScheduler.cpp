#include "TaskScheduler.h"
#include <iostream>

TaskScheduler::TaskScheduler()
    : running_(false) {}

TaskScheduler::~TaskScheduler()
{
    stop();
}

void TaskScheduler::add_task(const std::string &name, std::shared_ptr<Probe> probe, std::chrono::milliseconds interval)
{
    auto task = std::make_shared<Task>(name, probe, interval);
    {
        std::scoped_lock lock(mutex_);
        tasks_.push(task);
    }
    cv_.notify_one();
}

void TaskScheduler::start()
{
    if (!running_)
    {
        running_ = true;
        worker_ = std::jthread(&TaskScheduler::run, this);
    }
}

void TaskScheduler::resume()
{
    if (!running_)
    {
        running_ = true;
        cv_.notify_all();
        if (!worker_.joinable())
        {
            worker_ = std::jthread(&TaskScheduler::run, this);
        }
    }
}

void TaskScheduler::stop()
{
    running_ = false;
    cv_.notify_all();
    if (worker_.joinable())
    {
        worker_.join();
    }
}

void TaskScheduler::print_aggregates() const
{
    std::unique_lock lock(mutex_);

    auto temp_queue = tasks_;
    while (!temp_queue.empty())
    {
        const auto &task = temp_queue.top();
        const auto &agg = task->aggregator();
        const auto &probe = task->get_probe();
        for (const auto &type : agg.get_metric_types())
        {
            std::cout << "Task: " << task->name() << std::endl;
            std::cout << "Metric: " << probe->get_metric_name(type) << std::endl;
            std::cout << "Average: " << agg.average(type) << std::endl;
            std::cout << "Minimum: " << agg.minimum(type) << std::endl;
            std::cout << "Maximum: " << agg.maximum(type) << std::endl;
            std::cout << "Count: " << agg.count(type) << std::endl;
        }
        temp_queue.pop();
    }
}

std::unordered_map<Metric::Type, size_t> TaskScheduler::get_metric_counts() const
{
    std::unordered_map<Metric::Type, size_t> metric_counts;
    std::unique_lock lock(mutex_);

    auto temp_queue = tasks_;
    while (!temp_queue.empty())
    {
        const auto &task = temp_queue.top();
        const auto &agg = task->aggregator();
        for (const auto &type : agg.get_metric_types())
        {
            metric_counts[type] += agg.count(type);
        }
        temp_queue.pop();
    }

    return metric_counts;
}

void TaskScheduler::run()
{
    while (running_)
    {
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [this]
                 { return !tasks_.empty() || !running_; });

        while (!tasks_.empty() && running_)
        {
            auto now = std::chrono::high_resolution_clock::now();
            auto task = tasks_.top();
            if (now >= task->get_next_run_time())
            {
                tasks_.pop();
                lock.unlock();
                task->run();
                lock.lock();
                task->update_next_run_time(std::chrono::high_resolution_clock::now() + task->interval());
                tasks_.push(task);
            }
            else
            {
                cv_.wait_until(lock, task->get_next_run_time());
            }
        }
    }
    std::cout << "Scheduler loop exited" << std::endl;
}
