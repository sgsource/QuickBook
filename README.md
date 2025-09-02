# QuickBook

## Description

QuickBook is a high-performance, multi-threaded C++ order book implementation.
The `benchmark.cpp` executable runs simulated order placement to measure performance metrics such as throughput and latency across multiple threads. The benchmark outputs results as CSV files for analysis and plotting.

## Dependencies

This project uses the following third-party libraries:

* **Crow** – C++ microframework (header-only)
* **Asio** – Asynchronous I/O library for networking 
* **nlohmann/json** – Header-only JSON library

These can be downloaded from their official repositories or installed via package managers like `vcpkg` or `conan`. You can place headers in a `third_party/` folder.

## Folder Structure

```
QuickBook/
├── CMakeLists.txt
├── README.md
├── include/                # Header files for QuickBook library
│   ├── Order.h
│   ├── OrderBook.h
│   └── Trade.h
├── src/                    # Source files
│   ├── OrderBook.cpp
│   ├── benchmark.cpp       # Benchmarking executable
│   └── main.cpp
├── python/                 # Python scripts for plotting benchmark results
│   ├── plot_benchmarks.py
│   └── requirements.txt
├── results/                # CSV outputs from benchmark runs
│   └── benchmark_results.csv
└── plots/                  # Generated plots (PNG) from Python scripts
    ├── latency_vs_threads.png
    └── throughput_vs_threads.png

```

---

This version clearly separates the **library** from the **benchmark executable** and keeps the focus on what a recruiter or collaborator needs to know.

I can also draft a **“Getting Started” section** that explains how to build and run `benchmark.cpp` and generate plots from CSV if you want it fully polished for a portfolio. Do you want me to do that?
