#include "config.hpp"
#include <string>
#include <vector>
#include <map>

class OrderManager {
public:
    struct Order {
        std::string order_id;
        std::string instrument;
        double amount;
        double price;
        std::string side; // "buy" or "sell"
        std::string status; // "open", "filled", "cancelled"
    };

    struct Position {
        std::string instrument;
        double size;
        double average_price;
    };

    // Order management
    std::string place_order(const std::string& instrument, double amount, double price, const std::string& side) {
        Order new_order = {generate_order_id(), instrument, amount, price, side, "open"};
        active_orders_[new_order.order_id] = new_order;
        return new_order.order_id;
    }

    bool cancel_order(const std::string& order_id) {
        auto it = active_orders_.find(order_id);
        if (it != active_orders_.end()) {
            it->second.status = "cancelled";
            active_orders_.erase(it);
            return true;
        }
        return false;
    }

    bool modify_order(const std::string& order_id, double new_amount, double new_price) {
        auto it = active_orders_.find(order_id);
        if (it != active_orders_.end()) {
            it->second.amount = new_amount;
            it->second.price = new_price;
            return true;
        }
        return false;
    }
    
    // Position management
    std::vector<Position> get_positions() const;
    Position get_position(const std::string& instrument) const;

    // Risk management
    bool check_risk_limits(const std::string& instrument, double amount) const;

private:
    // Optimized concurrent data structures
    tbb::concurrent_hash_map<std::string, Order> active_orders_;
    tbb::concurrent_hash_map<std::string, Position> positions_;
    moodycamel::ConcurrentQueue<Order> order_pool_;

    // Object pool for order allocation
    Order allocate_order() {
        Order order;
        if (!order_pool_.try_dequeue(order)) {
            order = Order{};
        }
        return order;
    }

    void deallocate_order(Order&& order) {
        order_pool_.enqueue(std::move(order));
    }
};
