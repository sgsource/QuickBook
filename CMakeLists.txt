cmake_minimum_required(VERSION 3.10)
project(QuickBook)

# -----------------------
# C++ standard
# -----------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------
# Build type and optimization
# -----------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -pthread")

# -----------------------
# Warning option
# -----------------------
option(ENABLE_WARNINGS "Enable compiler warnings" ON)

if(ENABLE_WARNINGS)
    set(WARNING_FLAGS -Wall -Wextra)
else()
    set(WARNING_FLAGS "")
endif()

# -----------------------
# Include directories
# -----------------------
include_directories(
    include
    third_party
    third_party/asio/asio/include
)

# -----------------------
# Output directory
# -----------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# -----------------------
# Server executable
# -----------------------
add_executable(QuickBook
    src/main.cpp
    src/OrderBook.cpp
)

target_compile_options(QuickBook PRIVATE -O2 ${WARNING_FLAGS} -pthread)

# -----------------------
# Benchmark executable
# -----------------------
add_executable(Benchmark
    src/benchmark.cpp
    src/OrderBook.cpp
)

target_compile_options(Benchmark PRIVATE -O2 ${WARNING_FLAGS} -pthread)
