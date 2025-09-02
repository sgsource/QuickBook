#include "OrderBook.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <thread>
#include <chrono>

void run_benchmark(int num_threads, int orders_per_thread, std::ofstream& out) {
    OrderBook book;

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back([&book, orders_per_thread, t]() {
            for (int i = 0; i < orders_per_thread; i++) {
                Side side = (i % 2 == 0) ? Side::BUY : Side::SELL;
                double price = 100.0 + (i % 10);
                book.place_order("bot" + std::to_string(t), side, price, 1);
            }
        });
    }
    for (auto& th : threads) th.join();

    auto end = std::chrono::high_resolution_clock::now();
    double seconds = std::chrono::duration<double>(end - start).count();

    int total_orders = num_threads * orders_per_thread;
    double throughput = total_orders / seconds;
    double avg_latency_us = (seconds * 1e6) / total_orders;

    // Console print
    std::cout << num_threads << " threads | "
              << total_orders << " orders | "
              << seconds << " s | "
              << throughput << " orders/s | "
              << avg_latency_us << " us avg latency"
              << std::endl;

    // Write CSV row with Threads column
    out << num_threads << ","
        << total_orders << ","
        << seconds << ","
        << throughput << ","
        << avg_latency_us << "\n";
}

int main() {
    // Ensure results directory exists
    std::filesystem::create_directories("results");

    // Open CSV in results folder
    std::ofstream out("results/benchmark_results.csv");
    out << "Threads,Orders,TotalTime_s,Throughput_orders_per_s,AvgLatency_us\n";

    int orders_per_thread = 10000;
    std::vector<int> thread_counts = {1, 2, 4, 8, 16, 32, 64};

    for (int t : thread_counts) {
        run_benchmark(t, orders_per_thread, out);
    }

    out.close();
    std::cout << "Results saved to results/benchmark_results.csv\n";
    return 0;
}
