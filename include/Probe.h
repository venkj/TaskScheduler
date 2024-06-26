#ifndef PROBE_H
#define PROBE_H

#include "Metric.h"
#include <vector>
#include <string>

class Probe
{
public:
    virtual ~Probe() = default;

    // Collect metrics for the given types
    virtual std::vector<Metric> collect_metrics() = 0;

    // Get the name of the metric
    virtual std::string get_metric_name(Metric::Type type) const = 0;
};

#endif // PROBE_H
