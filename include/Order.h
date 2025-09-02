#pragma once
#include <string>

enum class Side { BUY, SELL };

struct Order {
    int id;
    std::string bot_name;
    Side side;
    double price;
    int quantity;
    Order() = default;
    Order(int id_, std::string bot, Side s, double p, int q)
        : id(id_), bot_name(bot), side(s), price(p), quantity(q) {}
};
