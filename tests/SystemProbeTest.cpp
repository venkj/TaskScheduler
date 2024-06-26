#include "SystemProbe.h"
#include "Metric.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <set>

class SystemProbeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        probe_ = std::make_shared<SystemProbe>();
    }

    std::shared_ptr<SystemProbe> probe_;
};

// Test to verify swapfile usage metric collection
TEST_F(SystemProbeTest, CollectMetrics_SwapfileUsage)
{
    auto metrics = probe_->collect_metrics();

    bool found = false;
    for (const auto &metric : metrics)
    {
        if (metric.type() == Metric::Type::SWAPFILE_USAGE)
        {
            found = true;
            EXPECT_GE(metric.value(), 0.0);
        }
    }
    EXPECT_TRUE(found);
}

// Test to verify memory usage metric collection
TEST_F(SystemProbeTest, CollectMetrics_MemoryUsage)
{
    auto metrics = probe_->collect_metrics();

    bool found = false;
    for (const auto &metric : metrics)
    {
        if (metric.type() == Metric::Type::MEMORY_USAGE)
        {
            found = true;
            EXPECT_GE(metric.value(), 0.0);
        }
    }
    EXPECT_TRUE(found);
}

// Test to verify network traffic metric collection
TEST_F(SystemProbeTest, CollectMetrics_NetworkTraffic)
{
    auto metrics = probe_->collect_metrics();

    bool found = false;
    for (const auto &metric : metrics)
    {
        if (metric.type() == Metric::Type::NETWORK_TRAFFIC)
        {
            found = true;
            EXPECT_GE(metric.value(), 0.0);
        }
    }
    EXPECT_TRUE(found);
}

// Test to verify collection of all metrics
TEST_F(SystemProbeTest, CollectAllMetrics)
{
    auto metrics = probe_->collect_metrics();

    std::set<Metric::Type> expected_metrics = {
        Metric::Type::SWAPFILE_USAGE,
        Metric::Type::MEMORY_USAGE,
        Metric::Type::NETWORK_TRAFFIC};

    std::set<Metric::Type> collected_metrics;
    for (const auto &metric : metrics)
    {
        collected_metrics.insert(metric.type());
        EXPECT_GE(metric.value(), 0.0);
    }

    EXPECT_EQ(collected_metrics, expected_metrics);
}

// Test to verify metric name retrieval
TEST_F(SystemProbeTest, GetMetricName)
{
    EXPECT_EQ(probe_->get_metric_name(Metric::Type::SWAPFILE_USAGE), "Swapfile Usage");
    EXPECT_EQ(probe_->get_metric_name(Metric::Type::MEMORY_USAGE), "Memory Usage");
    EXPECT_EQ(probe_->get_metric_name(Metric::Type::NETWORK_TRAFFIC), "Network Traffic");
}

// Test to verify invalid metric type throws exception
TEST_F(SystemProbeTest, InvalidMetricType)
{
    EXPECT_THROW(probe_->get_metric_name(static_cast<Metric::Type>(-1)), std::invalid_argument);
}
