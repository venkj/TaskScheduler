#ifndef NETWORK_PROBE_H
#define NETWORK_PROBE_H

#include "Probe.h"
#include "Metric.h"
#include <string>
class NetworkProbe : public Probe
{
public:
    NetworkProbe(const std::string &target);
    std::vector<Metric> collect_metrics() override;
    std::string get_metric_name(Metric::Type type) const override;

private:
    std::string target_;

    double dns_resolve_time() const;
    double tcp_connect_time() const;
    double icmp_ping_time() const;
    std::string resolve_hostname(const std::string &hostname) const;
};

#endif // NETWORK_PROBE_H
