#include "SystemProbe.h"
#include <stdexcept>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/vmmeter.h>
#include <mach/mach.h>
#include <mach/mach_init.h>
#include <mach/task.h>
#include <net/if.h>
#include <net/if_mib.h>
#include <net/if_dl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <ranges>
#include <algorithm>
#include <set>
#include <iostream>

std::vector<Metric> SystemProbe::collect_metrics()
{
    std::vector<Metric> metrics;
    collect_metric(metrics, Metric::Type::SWAPFILE_USAGE, [this]()
                   { return swapfile_usage(); }, [this](Metric::Type type)
                   { return get_metric_name(type); });

    collect_metric(metrics, Metric::Type::MEMORY_USAGE, [this]()
                   { return memory_usage(); }, [this](Metric::Type type)
                   { return get_metric_name(type); });

    collect_metric(metrics, Metric::Type::NETWORK_TRAFFIC, [this]()
                   { return network_traffic(); }, [this](Metric::Type type)
                   { return get_metric_name(type); });
    return metrics;
}

std::string SystemProbe::get_metric_name(Metric::Type type) const
{
    switch (type)
    {
    case Metric::Type::SWAPFILE_USAGE:
        return "Swapfile Usage";
    case Metric::Type::MEMORY_USAGE:
        return "Memory Usage";
    case Metric::Type::NETWORK_TRAFFIC:
        return "Network Traffic";
    default:
        throw std::invalid_argument("Unknown MetricType");
    }
}

double SystemProbe::swapfile_usage() const
{
    struct xsw_usage swap;
    size_t size = sizeof(swap);
    if (sysctlbyname("vm.swapusage", &swap, &size, nullptr, 0) != 0)
    {
        throw std::runtime_error("Failed to get swap usage");
    }
    return static_cast<double>(swap.xsu_used) / swap.xsu_total * 100.0;
}

double SystemProbe::memory_usage() const
{
    task_basic_info_data_t info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    const auto kerr = task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount);
    if (kerr != KERN_SUCCESS)
    {
        throw std::runtime_error("Failed to get memory usage");
    }

    return static_cast<double>(info.resident_size) / (1024 * 1024); // Convert to MB
}

double SystemProbe::network_traffic() const
{
    struct ifaddrs *ifaddr;
    unsigned long total_packets = 0;

    if (getifaddrs(&ifaddr) == -1)
    {
        throw std::runtime_error("Failed to get network interfaces");
    }

    for (struct ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == nullptr)
            continue;

        if (ifa->ifa_addr->sa_family == AF_LINK)
        {
            auto const *ifd = (struct if_data *)ifa->ifa_data;
            if (ifd != nullptr)
            {
                total_packets += ifd->ifi_ipackets + ifd->ifi_opackets;
            }
        }
    }

    freeifaddrs(ifaddr);
    return static_cast<double>(total_packets);
}