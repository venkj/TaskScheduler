#include "NetworkProbe.h"
#include "Metric.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <set>

class NetworkProbeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        valid_probe_ = std::make_shared<NetworkProbe>("google.com");
        invalid_probe_ = std::make_shared<NetworkProbe>("invalid.host");
    }

    std::shared_ptr<NetworkProbe> valid_probe_;
    std::shared_ptr<NetworkProbe> invalid_probe_;
};

// Test to verify DNS resolve time metric collection
TEST_F(NetworkProbeTest, CollectMetrics_DNSResolveTime)
{
    auto metrics = valid_probe_->collect_metrics();

    bool found = false;
    for (const auto &metric : metrics)
    {
        if (metric.type() == Metric::Type::DNS_RESOLVE_TIME)
        {
            found = true;
            EXPECT_GT(metric.value(), 0.0);
        }
    }
    EXPECT_TRUE(found);
}

// Test to verify TCP connect time metric collection
TEST_F(NetworkProbeTest, CollectMetrics_TCPConnectTime)
{
    auto metrics = valid_probe_->collect_metrics();

    bool found = false;
    for (const auto &metric : metrics)
    {
        if (metric.type() == Metric::Type::TCP_CONNECT_TIME)
        {
            found = true;
            EXPECT_GT(metric.value(), 0.0);
        }
    }
    EXPECT_TRUE(found);
}

// Test to verify collection of all metrics
TEST_F(NetworkProbeTest, CollectAllMetrics)
{
    auto metrics = valid_probe_->collect_metrics();

    std::set<Metric::Type> expected_metrics = {
        Metric::Type::DNS_RESOLVE_TIME,
        Metric::Type::TCP_CONNECT_TIME,
    };

    std::set<Metric::Type> collected_metrics;
    for (const auto &metric : metrics)
    {
        collected_metrics.insert(metric.type());
        EXPECT_GT(metric.value(), 0.0);
    }

    EXPECT_EQ(collected_metrics, expected_metrics);
}

// Test to verify metric name retrieval
TEST_F(NetworkProbeTest, GetMetricName)
{
    EXPECT_EQ(valid_probe_->get_metric_name(Metric::Type::DNS_RESOLVE_TIME), "DNS Resolve Time");
    EXPECT_EQ(valid_probe_->get_metric_name(Metric::Type::TCP_CONNECT_TIME), "TCP Connect Time");
}

// Test to verify invalid metric type throws exception
TEST_F(NetworkProbeTest, InvalidMetricType)
{
    EXPECT_THROW(valid_probe_->get_metric_name(static_cast<Metric::Type>(-1)), std::invalid_argument);
}

// Test to verify handling of invalid host
TEST_F(NetworkProbeTest, InvalidHost)
{
    auto metrics = invalid_probe_->collect_metrics();

    bool found_dns = false;
    bool found_tcp = false;
    bool found_icmp = false;

    for (const auto &metric : metrics)
    {
        if (metric.type() == Metric::Type::DNS_RESOLVE_TIME)
        {
            found_dns = true;
        }
        if (metric.type() == Metric::Type::TCP_CONNECT_TIME)
        {
            found_tcp = true;
        }
    }

    EXPECT_FALSE(found_dns); // DNS resolve should fail
    EXPECT_FALSE(found_tcp); // TCP connect should fail due to DNS failure
}
