#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <sstream>
#include "TaskScheduler.h"
#include "NetworkProbe.h"
#include "SystemProbe.h"

class TestableTaskScheduler : public TaskScheduler
{
public:
    using TaskScheduler::add_task;
    using TaskScheduler::get_metric_counts;
    using TaskScheduler::print_aggregates;
    using TaskScheduler::resume;
    using TaskScheduler::start;
    using TaskScheduler::stop;
};

class TaskSchedulerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        scheduler_ = std::make_unique<TestableTaskScheduler>();
        network_probe_ = std::make_shared<NetworkProbe>("google.com");
        system_probe_ = std::make_shared<SystemProbe>();
        scheduler_->add_task("network_test", network_probe_, std::chrono::milliseconds(200)); // Shorter interval for frequent execution
        scheduler_->add_task("system_test", system_probe_, std::chrono::milliseconds(200));   // Shorter interval for frequent execution
    }

    void TearDown() override
    {
        scheduler_->stop();
    }

    TestableTaskScheduler *scheduler() const
    {
        return scheduler_.get();
    }

private:
    std::unique_ptr<TestableTaskScheduler> scheduler_;
    std::shared_ptr<NetworkProbe> network_probe_;
    std::shared_ptr<SystemProbe> system_probe_;
};

TEST_F(TaskSchedulerTest, StartStopScheduler)
{
    scheduler()->start();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    scheduler()->stop();
    auto metric_counts = scheduler()->get_metric_counts();
    ASSERT_FALSE(metric_counts.empty());

    for (const auto &pair : metric_counts)
    {
        EXPECT_GT(pair.second, 0);
    }
}

TEST_F(TaskSchedulerTest, ResumeScheduler)
{
    scheduler()->start();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    scheduler()->stop();

    auto metric_counts_before = scheduler()->get_metric_counts();
    size_t count_network_before = metric_counts_before[Metric::Type::DNS_RESOLVE_TIME];
    size_t count_system_before = metric_counts_before[Metric::Type::MEMORY_USAGE];

    ASSERT_GT(count_network_before, 0);
    ASSERT_GT(count_system_before, 0);

    scheduler()->resume();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    scheduler()->stop();

    auto metric_counts_after = scheduler()->get_metric_counts();
    size_t count_network_after = metric_counts_after[Metric::Type::DNS_RESOLVE_TIME];
    size_t count_system_after = metric_counts_after[Metric::Type::MEMORY_USAGE];

    EXPECT_GT(count_network_after, count_network_before);
    EXPECT_GT(count_system_after, count_system_before);
}

TEST_F(TaskSchedulerTest, AddTaskWhileRunning)
{
    scheduler()->start();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto additional_probe = std::make_shared<NetworkProbe>("google.com");
    scheduler()->add_task("Additional test task", additional_probe, std::chrono::milliseconds(200));

    std::this_thread::sleep_for(std::chrono::seconds(2));
    scheduler()->stop();

    auto metric_counts = scheduler()->get_metric_counts();
    ASSERT_FALSE(metric_counts.empty());

    for (const auto &pair : metric_counts)
    {
        EXPECT_GT(pair.second, 0);
    }
}

TEST_F(TaskSchedulerTest, TaskExecutionIntervals)
{
    scheduler()->start();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    scheduler()->stop();

    auto metric_counts = scheduler()->get_metric_counts();
    ASSERT_FALSE(metric_counts.empty());

    // Verify that each metric type was collected more than a minimum expected number of times
    for (const auto &pair : metric_counts)
    {
        EXPECT_GT(pair.second, 3);
    }
}

TEST_F(TaskSchedulerTest, PrintAggregates)
{
    scheduler()->start();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    scheduler()->stop();

    std::ostringstream oss;
    std::streambuf *p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());

    scheduler()->print_aggregates();

    std::cout.rdbuf(p_cout_streambuf);

    std::string output = oss.str();
    std::cout << "Captured Output: " << output << std::endl;

    EXPECT_THAT(output, ::testing::HasSubstr("Metric: DNS Resolve Time"));
    EXPECT_THAT(output, ::testing::HasSubstr("Metric: Memory Usage"));
    EXPECT_THAT(output, ::testing::HasSubstr("Average:"));
    EXPECT_THAT(output, ::testing::HasSubstr("Minimum:"));
    EXPECT_THAT(output, ::testing::HasSubstr("Maximum:"));
}
