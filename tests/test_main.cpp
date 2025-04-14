#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../include/config.hpp"
#include "../src/deribit_api/client.hpp"
#include "../src/order_manager/manager.hpp"

TEST_CASE("Configuration Constants", "[config]") {
    SECTION("API URLs") {
        REQUIRE(DERIBIT_API_URL == "https://test.deribit.com/api/v2/");
        REQUIRE(WEBSOCKET_URL == "wss://test.deribit.com/ws/api/v2/");
    }
}

TEST_CASE("Order Manager", "[order]") {
    OrderManager manager;
    
    SECTION("Order Placement") {
        auto order_id = manager.place_order("BTC-PERPETUAL", 0.1, 50000, "buy");
        REQUIRE_FALSE(order_id.empty());
    }
    
    SECTION("Order Cancellation") {
        auto order_id = manager.place_order("BTC-PERPETUAL", 0.1, 50000, "buy");
        REQUIRE(manager.cancel_order(order_id));
    }
}

TEST_CASE("Deribit Client", "[api]") {
    DeribitClient client("test_id", "test_secret");
    
    SECTION("Authentication") {
        // Mock test - actual API calls would need mocking
        REQUIRE_NOTHROW(client.authenticate());
    }
}

TEST_CASE("Performance Benchmark", "[benchmark]") {
    OrderManager manager;
    constexpr size_t kIterations = 10000;
    
    BENCHMARK("Order Placement Throughput") {
        for (size_t i = 0; i < kIterations; ++i) {
            manager.place_order("BTC-PERPETUAL", 0.1, 50000, "buy");
        }
    };
    
    BENCHMARK("Order Cancellation Throughput") {
        std::vector<std::string> order_ids;
        for (size_t i = 0; i < kIterations; ++i) {
            order_ids.push_back(manager.place_order("BTC-PERPETUAL", 0.1, 50000, "buy"));
        }
        for (const auto& id : order_ids) {
            manager.cancel_order(id);
        }
    };
}
