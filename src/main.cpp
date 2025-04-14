#include <iostream>
#include "config.hpp"
#include "deribit_api/client.hpp"
#include "order_manager/manager.hpp"
#include "market_data/server.hpp"

int main() {
    try {
        // Initialize components
        DeribitClient api_client("your_client_id", "your_client_secret");
        OrderManager order_manager;
        MarketDataServer data_server(9000);

        // Authenticate with Deribit
        if (!api_client.authenticate()) {
            std::cerr << "Failed to authenticate with Deribit API" << std::endl;
            return 1;
        }

        // Start market data server
        if (!data_server.start()) {
            std::cerr << "Failed to start market data server" << std::endl;
            return 1;
        }

        std::cout << "Deribit Trading System successfully initialized" << std::endl;
        
        // Benchmarking setup
        auto start_time = std::chrono::steady_clock::now();
        size_t order_count = 0;
        size_t data_messages = 0;
        std::vector<uint64_t> order_latencies;
        std::vector<uint64_t> data_latencies;

        // Main application loop
        while (true) {
            auto loop_start = std::chrono::steady_clock::now();
            
            // Process market data
            auto data_start = std::chrono::steady_clock::now();
            // ... data processing logic ...
            auto data_duration = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - data_start);
            data_latencies.push_back(data_duration.count());
            data_messages++;

            // Process orders
            auto order_start = std::chrono::steady_clock::now();
            // ... order processing logic ...
            auto order_duration = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - order_start);
            order_latencies.push_back(order_duration.count());
            order_count++;

            // Periodic reporting
            auto now = std::chrono::steady_clock::now();
            if (now - start_time > std::chrono::seconds(10)) {
                auto avg_data = std::accumulate(data_latencies.begin(), data_latencies.end(), 0) / data_latencies.size();
                auto avg_order = std::accumulate(order_latencies.begin(), order_latencies.end(), 0) / order_latencies.size();
                
                std::cout << "Performance Metrics (last 10s):\n"
                          << "  Market data latency: " << avg_data << "μs avg (" 
                          << *std::min_element(data_latencies.begin(), data_latencies.end()) << "μs min, "
                          << *std::max_element(data_latencies.begin(), data_latencies.end()) << "μs max)\n"
                          << "  Order latency: " << avg_order << "μs avg ("
                          << *std::min_element(order_latencies.begin(), order_latencies.end()) << "μs min, "
                          << *std::max_element(order_latencies.begin(), order_latencies.end()) << "μs max)\n"
                          << "  Throughput: " << (data_messages/10) << " msg/s, " 
                          << (order_count/10) << " orders/s\n";
                
                start_time = now;
                order_count = 0;
                data_messages = 0;
                order_latencies.clear();
                data_latencies.clear();
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
