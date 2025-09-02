import pandas as pd
import matplotlib.pyplot as plt
import os

# Define paths
BASE_DIR = os.path.join(os.path.dirname(__file__), "..")
RESULTS_DIR = os.path.join(BASE_DIR, "results")
PLOTS_DIR = os.path.join(BASE_DIR, "plots")

# Create directories if they don't exist
os.makedirs(RESULTS_DIR, exist_ok=True)
os.makedirs(PLOTS_DIR, exist_ok=True)

# Load CSV from results folder
csv_path = os.path.join(RESULTS_DIR, "benchmark_results.csv")
df = pd.read_csv(csv_path)

# Strip whitespace from column names
df.columns = df.columns.str.strip()

# -------------------------------
# Plot throughput
plt.figure(figsize=(8,5))
plt.plot(df["Threads"], df["Throughput_orders_per_s"], marker="o")
plt.title("Throughput vs Threads")
plt.xlabel("Threads")
plt.ylabel("Throughput (orders/sec)")
plt.grid(True)
plt.savefig(os.path.join(PLOTS_DIR, "throughput_vs_threads.png"))

# -------------------------------
# Plot latency
plt.figure(figsize=(8,5))
plt.plot(df["Threads"], df["AvgLatency_us"], marker="o", color="red")
plt.title("Average Latency vs Threads")
plt.xlabel("Threads")
plt.ylabel("Latency (µs)")
plt.grid(True)
plt.savefig(os.path.join(PLOTS_DIR, "latency_vs_threads.png"))