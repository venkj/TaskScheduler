#include "NetworkProbe.h"
#include <boost/asio.hpp>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <ranges>
#include <algorithm>
#include <set>

NetworkProbe::NetworkProbe(const std::string &target)
    : target_(target)
{
}

std::vector<Metric> NetworkProbe::collect_metrics()
{
    std::vector<Metric> metrics;
    collect_metric(metrics, Metric::Type::DNS_RESOLVE_TIME, [this]()
                   { return dns_resolve_time(); }, [this](Metric::Type type)
                   { return get_metric_name(type); });

    collect_metric(metrics, Metric::Type::TCP_CONNECT_TIME, [this]()
                   { return tcp_connect_time(); }, [this](Metric::Type type)
                   { return get_metric_name(type); });

    // collect_metric(metrics, Metric::Type::ICMP_PING_TIME, [this]() { return icmp_ping_time(); }, [this](Metric::Type type) { return get_metric_name(type); });
    return metrics;
}

std::string NetworkProbe::get_metric_name(Metric::Type type) const
{
    switch (type)
    {
    case Metric::Type::DNS_RESOLVE_TIME:
        return "DNS Resolve Time";
    case Metric::Type::TCP_CONNECT_TIME:
        return "TCP Connect Time";
    default:
        throw std::invalid_argument("Unknown MetricType");
    }
}

double NetworkProbe::dns_resolve_time() const
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::system::error_code ec;

    auto start = std::chrono::high_resolution_clock::now();
    auto results = resolver.resolve(target_, "", ec);
    if (ec)
    {
        throw std::runtime_error("Failed to resolve DNS: " + ec.message());
    }
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double, std::milli>(end - start).count();
}

double NetworkProbe::tcp_connect_time() const
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::query query(target_, "80");
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    boost::asio::ip::tcp::socket socket(io_context);

    auto start = std::chrono::high_resolution_clock::now();
    boost::asio::connect(socket, endpoint_iterator);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double, std::milli>(end - start).count();
}

double NetworkProbe::icmp_ping_time() const
{
    std::string ip_address = resolve_hostname(target_);
    boost::asio::io_context io_context;
    boost::asio::ip::icmp::resolver resolver(io_context);

    boost::asio::ip::icmp::resolver::query query(boost::asio::ip::icmp::v4(), ip_address, "");
    boost::asio::ip::icmp::endpoint destination = *resolver.resolve(query);

    boost::asio::ip::icmp::socket socket(io_context, boost::asio::ip::icmp::v4());

    // ICMP request packet
    std::vector<uint8_t> request(8, 0);
    request[0] = 8; // Type: ICMP Echo Request

    try
    {
        auto start = std::chrono::high_resolution_clock::now();
        socket.send_to(boost::asio::buffer(request), destination);

        std::vector<uint8_t> reply(1024);
        boost::asio::ip::icmp::endpoint sender_endpoint;
        socket.receive_from(boost::asio::buffer(reply), sender_endpoint);

        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    }
    catch (const boost::system::system_error &e)
    {
        throw std::system_error(e.code());
    }
}

std::string NetworkProbe::resolve_hostname(const std::string &hostname) const
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::system::error_code ec;

    auto results = resolver.resolve(hostname, "", ec);
    if (ec)
    {
        throw std::runtime_error("Failed to resolve hostname: " + ec.message());
    }

    return results->endpoint().address().to_string();
}