#pragma once
#include "Order.h"
#include "Trade.h"
#include <set>
#include <vector>
#include <mutex>

struct OrderComparator {
    bool operator()(const Order& a, const Order& b) const {
        // Sort descending for bids
        return a.price > b.price || (a.price == b.price && a.id < b.id);
    }
};

struct AskComparator {
    bool operator()(const Order& a, const Order& b) const {
        // Sort ascending for asks
        return a.price < b.price || (a.price == b.price && a.id < b.id);
    }
};

class OrderBook {
private:
    std::mutex bids_mtx;
    std::mutex asks_mtx;
    int next_order_id = 1;
    int next_trade_id = 1;

    std::multiset<Order, OrderComparator> bids;
    std::multiset<Order, AskComparator> asks;
    std::vector<Trade> trades;
    std::mutex trades_mtx;

    void match_orders();

public:
    int place_order(const std::string& bot, Side side, double price, int quantity);
    bool cancel_order(int order_id);
    std::vector<Order> get_bids();
    std::vector<Order> get_asks();
    std::vector<Trade> get_trades();
};
