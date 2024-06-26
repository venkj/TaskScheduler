#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include "Task.h"
#include "Probe.h"
#include <vector>
#include <map>
#include <functional>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>

class TaskScheduler
{
public:
    TaskScheduler();
    ~TaskScheduler();

    void add_task(const std::string &name, std::shared_ptr<Probe> probe, std::chrono::milliseconds interval);
    void start();
    void resume();
    void stop();

    void print_aggregates() const;

protected:
    // Used in testing
    std::unordered_map<Metric::Type, size_t> get_metric_counts() const;

private:
    void run();

    std::atomic<bool> running_;
    std::jthread worker_;
    std::condition_variable cv_;
    mutable std::mutex mutex_;

    using TaskPtr = std::shared_ptr<Task>;

    struct TaskComparator
    {
        bool operator()(const TaskPtr &lhs, const TaskPtr &rhs) const
        {
            return lhs->get_next_run_time() > rhs->get_next_run_time();
        }
    };

    std::priority_queue<TaskPtr, std::vector<TaskPtr>, TaskComparator> tasks_;
};

#endif // TASK_SCHEDULER_H
