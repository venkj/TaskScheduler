#include "MetricAggregator.h"

void MetricAggregator::add_metric(const Metric &metric)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto &data = data_[metric.type()];

    data.sum += metric.value();
    data.min = std::min(data.min, metric.value());
    data.max = std::max(data.max, metric.value());
    data.count++;
}

double MetricAggregator::average(Metric::Type type) const
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(type);
    if (it != data_.end() && it->second.count > 0)
    {
        return it->second.sum / it->second.count;
    }
    return 0.0;
}

double MetricAggregator::minimum(Metric::Type type) const
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(type);
    if (it != data_.end())
    {
        return it->second.min;
    }
    return std::numeric_limits<double>::max();
}

double MetricAggregator::maximum(Metric::Type type) const
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(type);
    if (it != data_.end())
    {
        return it->second.max;
    }
    return std::numeric_limits<double>::lowest();
}

size_t MetricAggregator::count(Metric::Type type) const
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(type);
    if (it != data_.end())
    {
        return it->second.count;
    }
    return 0;
}

std::vector<Metric::Type> MetricAggregator::get_metric_types() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<Metric::Type> types;
    for (const auto &pair : data_)
    {
        types.push_back(pair.first);
    }
    return types;
}
