#pragma once
#include <string>

struct Trade {
    int trade_id;
    int buy_order_id;
    int sell_order_id;
    std::string buyer;
    std::string seller;
    double price;
    int quantity;
    Trade(int tid, int buy_id, int sell_id, std::string b, std::string s, double p, int q)
        : trade_id(tid), buy_order_id(buy_id), sell_order_id(sell_id),
          buyer(b), seller(s), price(p), quantity(q) {}
};
