#include "OrderBook.h"
#include <algorithm>

int OrderBook::place_order(const std::string& bot, Side side, double price, int quantity) {
    int id = 0;
    if (side == Side::BUY) {
        {
            std::lock_guard<std::mutex> lock(bids_mtx);
            id = next_order_id++;
            Order o = Order(id, bot, side, price, quantity);
            bids.insert(o);
        }
    } else {
        {
            std::lock_guard<std::mutex> lock(asks_mtx);
            id = next_order_id++;
            Order o = Order(id, bot, side, price, quantity);
            asks.insert(o);
        }
    }

    match_orders(); // minimal locking inside
    return id;
}

bool OrderBook::cancel_order(int order_id) {
    bool removed = false;
    {
        std::lock_guard<std::mutex> lock(bids_mtx);
        auto it = std::find_if(bids.begin(), bids.end(), [order_id](const Order& o){ return o.id == order_id; });
        if (it != bids.end()) { bids.erase(it); removed = true; }
    }
    {
        std::lock_guard<std::mutex> lock(asks_mtx);
        auto it = std::find_if(asks.begin(), asks.end(), [order_id](const Order& o){ return o.id == order_id; });
        if (it != asks.end()) { asks.erase(it); removed = true; }
    }
    return removed;
}

std::vector<Order> OrderBook::get_bids() {
    std::lock_guard<std::mutex> lock(bids_mtx);
    return std::vector<Order>(bids.begin(), bids.end());
}

std::vector<Order> OrderBook::get_asks() {
    std::lock_guard<std::mutex> lock(asks_mtx);
    return std::vector<Order>(asks.begin(), asks.end());
}

std::vector<Trade> OrderBook::get_trades() {
    std::lock_guard<std::mutex> lock(trades_mtx);
    return trades;
}

void OrderBook::match_orders() {
    while (true) {
        Order buy;
        Order sell;
        {
            std::lock_guard<std::mutex> lock_b(bids_mtx);
            std::lock_guard<std::mutex> lock_a(asks_mtx);
            if (bids.empty() || asks.empty()) break;
            auto bid_it = bids.begin();
            auto ask_it = asks.begin();
            if (bid_it->price < ask_it->price) break;

            buy = *bid_it;
            sell = *ask_it;
            bids.erase(bid_it);
            asks.erase(ask_it);
        }

        int trade_qty = std::min(buy.quantity, sell.quantity);
        double trade_price = sell.price;

        {
            std::lock_guard<std::mutex> lock(trades_mtx);
            trades.emplace_back(next_trade_id++, buy.id, sell.id, buy.bot_name, sell.bot_name, trade_price, trade_qty);
        }

        // reinsert leftover quantities
        if (buy.quantity > trade_qty) {
            buy.quantity -= trade_qty;
            std::lock_guard<std::mutex> lock(bids_mtx);
            bids.insert(buy);
        }
        if (sell.quantity > trade_qty) {
            sell.quantity -= trade_qty;
            std::lock_guard<std::mutex> lock(asks_mtx);
            asks.insert(sell);
        }
    }
}
