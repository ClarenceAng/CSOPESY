#pragma once

#include <iostream>
#include <string>
#include <format>
#include <cstdint>
#include <functional>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include <unordered_map>
#include <queue>
#include <tuple>

#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include <shared_mutex>

enum SchedulerType {
    FCFS,
    RR
};

using SymbolTable = std::unordered_map<std::string, uint16_t>;
using Logger = std::vector<std::tuple<std::string, std::string>>;

struct Config {
    uint8_t numCpu;
    SchedulerType schedulerType;
    uint64_t quantumCycles;
    uint64_t batchProcessFreq;
    uint64_t minIns;
    uint64_t maxIns;
    uint64_t delaysPerExec;
};

inline Config config;

inline void headerDisplay() {
    // ASCII Art Generator: https://patorjk.com/software/taag/
    std::cout << R"( _____  _____  ___________ _____ _______   __)" << std::endl
              << R"(/  __ \/  ___||  _  | ___ \  ___/  ___\ \ / /)" << std::endl
              << R"(| /  \/\ `--. | | | | |_/ / |__ \ `--. \ V /)" << std::endl
              << R"(| |     `--. \| | | |  __/|  __| `--. \ \ /)" << std::endl
              << R"(| \__/\/\__/ /\ \_/ / |   | |___/\__/ / | |)" << std::endl
              << R"( \____/\____/  \___/\_|   \____/\____/  \_/)" << std::endl
              << "----------------------------------------------" << std::endl
              << "Welcome to CSOPESY Emulator!                  " << std::endl
              << "                                              " << std::endl
              << "Developers:                                   " << std::endl
              << "Adiong, Nathaniel Irvin L.                    " << std::endl
              << "Ang, Clarence Ivan C.                         " << std::endl
              << "Go, Kenneth D.                                " << std::endl
              << "Trocino, Job D.                               " << std::endl
              << "                                              " << std::endl
              << "Last updated: 06-16-2026                      " << std::endl
              << "----------------------------------------------" << std::endl
              ;
}