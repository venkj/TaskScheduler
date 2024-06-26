#ifndef SYSTEM_PROBE_H
#define SYSTEM_PROBE_H

#include "Probe.h"
#include "Metric.h"

class SystemProbe : public Probe
{
public:
    SystemProbe() = default;
    std::vector<Metric> collect_metrics() override;
    std::string get_metric_name(Metric::Type type) const override;

private:
    double swapfile_usage() const;
    double memory_usage() const;
    double network_traffic() const;
};

#endif // SYSTEM_PROBE_H
