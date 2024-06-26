#ifndef METRIC_AGGREGATOR_H
#define METRIC_AGGREGATOR_H

#include "Metric.h"
#include <unordered_map>
#include <mutex>
#include <limits>
#include <algorithm>
#include <vector>

// Class responsible for aggregating metrics
class MetricAggregator
{
private:
    // Struct to store aggregated data for each metric type
    struct AggregatedData
    {
        double sum = 0.0;
        double min = std::numeric_limits<double>::max();
        double max = std::numeric_limits<double>::lowest();
        size_t count = 0;
    };

public:
    MetricAggregator() = default;

    // Add a metric to the aggregator
    void add_metric(const Metric &metric);

    // Get aggregate values for a specific metric type
    double average(Metric::Type type) const;
    double minimum(Metric::Type type) const;
    double maximum(Metric::Type type) const;
    size_t count(Metric::Type type) const;

    // Get the list of metric types that have been aggregated
    std::vector<Metric::Type> get_metric_types() const;

private:
    mutable std::mutex mutex_;                              // Mutex for thread-safe access
    std::unordered_map<Metric::Type, AggregatedData> data_; // Aggregated data storage
};

#endif // METRIC_AGGREGATOR_H
