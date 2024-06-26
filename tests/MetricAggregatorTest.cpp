#include "MetricAggregator.h"
#include "Metric.h"
#include <gtest/gtest.h>

class MetricAggregatorTest : public ::testing::Test
{
protected:
    MetricAggregator aggregator_;
};

// Test to verify that adding a single metric updates the aggregator correctly
TEST_F(MetricAggregatorTest, AddSingleMetric)
{
    Metric metric(Metric::Type::DNS_RESOLVE_TIME, 100.0);
    aggregator_.add_metric(metric);

    EXPECT_EQ(aggregator_.count(Metric::Type::DNS_RESOLVE_TIME), 1);
    EXPECT_EQ(aggregator_.average(Metric::Type::DNS_RESOLVE_TIME), 100.0);
    EXPECT_EQ(aggregator_.minimum(Metric::Type::DNS_RESOLVE_TIME), 100.0);
    EXPECT_EQ(aggregator_.maximum(Metric::Type::DNS_RESOLVE_TIME), 100.0);
}

// Test to verify that adding multiple metrics updates the aggregator correctly
TEST_F(MetricAggregatorTest, AddMultipleMetrics)
{
    Metric metric1(Metric::Type::DNS_RESOLVE_TIME, 100.0);
    Metric metric2(Metric::Type::DNS_RESOLVE_TIME, 200.0);
    Metric metric3(Metric::Type::DNS_RESOLVE_TIME, 150.0);

    aggregator_.add_metric(metric1);
    aggregator_.add_metric(metric2);
    aggregator_.add_metric(metric3);

    EXPECT_EQ(aggregator_.count(Metric::Type::DNS_RESOLVE_TIME), 3);
    EXPECT_EQ(aggregator_.average(Metric::Type::DNS_RESOLVE_TIME), 150.0);
    EXPECT_EQ(aggregator_.minimum(Metric::Type::DNS_RESOLVE_TIME), 100.0);
    EXPECT_EQ(aggregator_.maximum(Metric::Type::DNS_RESOLVE_TIME), 200.0);
}

// Test to verify that adding metrics of different types updates the aggregator correctly
TEST_F(MetricAggregatorTest, AddMetricsOfDifferentTypes)
{
    Metric metric1(Metric::Type::DNS_RESOLVE_TIME, 100.0);
    Metric metric2(Metric::Type::TCP_CONNECT_TIME, 200.0);

    aggregator_.add_metric(metric1);
    aggregator_.add_metric(metric2);

    EXPECT_EQ(aggregator_.count(Metric::Type::DNS_RESOLVE_TIME), 1);
    EXPECT_EQ(aggregator_.average(Metric::Type::DNS_RESOLVE_TIME), 100.0);
    EXPECT_EQ(aggregator_.minimum(Metric::Type::DNS_RESOLVE_TIME), 100.0);
    EXPECT_EQ(aggregator_.maximum(Metric::Type::DNS_RESOLVE_TIME), 100.0);

    EXPECT_EQ(aggregator_.count(Metric::Type::TCP_CONNECT_TIME), 1);
    EXPECT_EQ(aggregator_.average(Metric::Type::TCP_CONNECT_TIME), 200.0);
    EXPECT_EQ(aggregator_.minimum(Metric::Type::TCP_CONNECT_TIME), 200.0);
    EXPECT_EQ(aggregator_.maximum(Metric::Type::TCP_CONNECT_TIME), 200.0);
}

// Test to verify that no metrics returns empty data
TEST_F(MetricAggregatorTest, NoMetrics)
{
    auto types = aggregator_.get_metric_types();
    EXPECT_TRUE(types.empty());
}
