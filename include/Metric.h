#ifndef METRIC_H
#define METRIC_H

#include <string>
#include <vector>
#include <iostream>
#include <functional>

class Metric
{
public:
    enum class Type
    {
        // Used in SystemProbe class
        SWAPFILE_USAGE,
        MEMORY_USAGE,
        NETWORK_TRAFFIC,

        // Used in NetworkProbe class
        DNS_RESOLVE_TIME,
        TCP_CONNECT_TIME,
        ICMP_PING_TIME,
    };

    Metric(Type type, double value) : type_(type), value_(value) {}

    Type type() const { return type_; };
    double value() const { return value_; };

private:
    Type type_;
    double value_;
};

// Helper function to safely collect a metric
template <typename Func>
void collect_metric(std::vector<Metric> &metrics,
                    Metric::Type type, Func func,
                    std::function<std::string(Metric::Type)> get_metric_name)
{
    try
    {
        metrics.emplace_back(type, func());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to collect " << get_metric_name(type) << ": " << e.what() << std::endl;
    }
}

#endif // METRIC_H
