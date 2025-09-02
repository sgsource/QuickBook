#include "crow_all.h"
#include "OrderBook.h"
#include "json.hpp"

using json = nlohmann::json;

int main() {
    crow::SimpleApp app;
    OrderBook book;

    CROW_ROUTE(app, "/place_order").methods("POST"_method)(
        [&book](const crow::request& req){
            auto j = json::parse(req.body);
            std::string bot = j["bot"];
            std::string side_str = j["side"];
            double price = j["price"];
            int quantity = j["quantity"];
            Side side = (side_str == "buy") ? Side::BUY : Side::SELL;
            int order_id = book.place_order(bot, side, price, quantity);
            json res = { {"status","ok"}, {"order_id", order_id} };
            return crow::response(res.dump());
        }
    );

    CROW_ROUTE(app, "/cancel_order").methods("POST"_method)(
        [&book](const crow::request& req){
            auto j = json::parse(req.body);
            int order_id = j["order_id"];
            bool ok = book.cancel_order(order_id);
            json res = { {"status", ok ? "cancelled" : "failed"} };
            return crow::response(res.dump());
        }
    );

    CROW_ROUTE(app, "/book").methods("GET"_method)(
        [&book](const crow::request&){
            json res;
            res["bids"] = json::array();
            for(auto& o : book.get_bids())
                res["bids"].push_back({ {"id", o.id}, {"bot", o.bot_name}, {"price", o.price}, {"qty", o.quantity} });
            res["asks"] = json::array();
            for(auto& o : book.get_asks())
                res["asks"].push_back({ {"id", o.id}, {"bot", o.bot_name}, {"price", o.price}, {"qty", o.quantity} });
            return crow::response(res.dump());
        }
    );

    CROW_ROUTE(app, "/trades").methods("GET"_method)(
        [&book](const crow::request&){
            json res = json::array();
            for(auto& t : book.get_trades())
                res.push_back({
                    {"trade_id", t.trade_id},
                    {"buyer", t.buyer},
                    {"seller", t.seller},
                    {"price", t.price},
                    {"quantity", t.quantity}
                });
            return crow::response(res.dump());
        }
    );

    app.port(5000).multithreaded().run();
}
