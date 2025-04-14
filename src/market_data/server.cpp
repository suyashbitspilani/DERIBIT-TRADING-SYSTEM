#include "config.hpp"
#include <string>
#include <vector>
#include <set>
#include <functional>

class MarketDataServer {
public:
    using SubscriptionCallback = std::function<void(const std::string& symbol, const std::string& data)>;

    MarketDataServer(int port = 9000);
    ~MarketDataServer();

    // Server control
    bool start() {
        if (running_) return true;
        
        try {
            // Initialize WebSocket server
            server_.init_asio();
            server_.set_open_handler([this](websocketpp::connection_hdl hdl) {
                connection_handler_.add_connection(hdl);
            });
            server_.set_close_handler([this](websocketpp::connection_hdl hdl) {
                connection_handler_.remove_connection(hdl);
            });
            
            server_.listen(port_);
            server_.start_accept();
            server_thread_ = std::thread([this]() {
                running_ = true;
                server_.run();
            });
            return true;
        } catch (...) {
            return false;
        }
    }

    void stop() {
        if (!running_) return;
        
        server_.stop();
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
        running_ = false;
    }

    // Subscription management
    void subscribe(const std::string& client_id, const std::vector<std::string>& symbols);
    void unsubscribe(const std::string& client_id, const std::vector<std::string>& symbols);

    // Data distribution
    void broadcast(const std::string& symbol, const std::string& data);

    // Callback registration
    void set_callback(SubscriptionCallback callback);

private:
    int port_;
    bool running_;
    std::set<std::string> subscribed_symbols_;
    std::map<std::string, std::set<std::string>> client_subscriptions_;
    SubscriptionCallback callback_;
};
