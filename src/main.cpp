#include <fstream>

#include "globals.h"
#include "ConsoleManager.h"
#include "Scheduler.h"

bool initialize() {
    // Look for config.txt relative to wherever the program is launched from.
    const std::vector<std::string> configPaths = { "src/config.txt" };

    std::ifstream file;
    for (const auto& path : configPaths) {
        file.open(path);
        if (file.is_open()) break;
    }

    if (!file.is_open()) {
        std::cerr << "Error: Could not open config.txt\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string key, value;

        if (ss >> key >> value) {
            if (key == "num-cpu") config.numCpu = std::stoi(value);
            if (key == "scheduler") {
                if (value == "\"fcfs\"") config.schedulerType = FCFS;
                if (value == "\"rr\"") config.schedulerType = RR;
            }
            if (key == "quantum-cycles") config.quantumCycles = std::stoi(value);
            if (key == "batch-process-freq") config.batchProcessFreq = std::stoi(value);
            if (key == "min-ins") config.minIns = std::stoi(value);
            if (key == "max-ins") config.maxIns = std::stoi(value);
            if (key == "delays-per-exec") config.delaysPerExec = std::stoi(value);
        }
    }

    // Instantiate the rest of the program now that config is loaded.
    ConsoleManager::initialize();
    Scheduler::initialize();

    return true;
}

int main() {
    std::cout << "\033[2J\033[H";
    headerDisplay();

    // Boot loop: only 'initialize' and 'exit' are accepted until we're up.
    bool initialized = false;
    std::string cmd;
    while (true) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, cmd);

        if (cmd == "exit") {
            std::cout << "Exiting...\n";
            break;
        }
        if (cmd == "initialize") {
            initialized = initialize();
            if (initialized) break;
            continue;
        }
        std::cerr << "Error: Please run 'initialize' first.\n";
    }

    if (!initialized) {
        std::cout << "Program stopped successfully.\n";
        return 0;
    }

    ConsoleManager::getInstance()->start();

    while (ConsoleManager::getInstance()->isRunning()) {}

    ConsoleManager::getInstance()->stop();
    Scheduler::getInstance()->stop();
    ConsoleManager::destroy();
    Scheduler::destroy();

    std::cout << "Program stopped successfully.\n";

    return 0;
}
