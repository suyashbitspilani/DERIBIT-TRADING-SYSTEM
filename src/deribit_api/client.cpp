#include "config.hpp"
#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "http_client.hpp"  // Assuming this is where http_client_ is defined

class DeribitClient {
public:
    DeribitClient(const std::string& client_id, const std::string& client_secret)
        : client_id_(client_id), client_secret_(client_secret) {}

    // Authentication methods
    bool authenticate() {
        // Implementation of OAuth2 authentication flow
        try {
            // Make HTTP POST request to Deribit auth endpoint
            auto response = http_client_.post(
                DERIBIT_API_URL + "public/auth",
                {{"grant_type", "client_credentials"},
                 {"client_id", client_id_},
                 {"client_secret", client_secret_}});
            
            if (response.status_code() == 200) {
                auto json = nlohmann::json::parse(response.body());
                access_token_ = json["access_token"];
                refresh_token_ = json["refresh_token"];
                return true;
            }
            return false;
        } catch (...) {
            return false;
        }
    }

    void refresh_token() {
        try {
            // Make HTTP POST request to refresh token
            auto response = http_client_.post(
                DERIBIT_API_URL + "public/auth",
                {{"grant_type", "refresh_token"},
                 {"refresh_token", refresh_token_}});
            
            if (response.status_code() == 200) {
                auto json = nlohmann::json::parse(response.body());
                access_token_ = json["access_token"];
                refresh_token_ = json["refresh_token"];
            }
        } catch (...) {
            // Log refresh failure
        }
    }

    // API methods
    std::string get_account_summary();
    std::string place_order(const std::string& instrument, double amount, double price);
    std::string cancel_order(const std::string& order_id);
    std::string get_open_orders();
    std::string get_positions();

private:
    std::string client_id_;
    std::string client_secret_;
    std::string access_token_;
    std::string refresh_token_;
};
