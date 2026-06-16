#include "Bootstrap.h"
#include "ConsoleManager.h"
#include "Scheduler.h"

Bootstrap::Bootstrap() {
    commands = {
        { "initialize", [this](const auto& args) {
            if (!args.empty()) {
                std::cerr << "Error: initialize takes no arguments." << std::endl;
                return;
            }

            initialize();
        }},

        { "exit", [this](const auto& args) {
            if (!args.empty()) {
                std::cerr << "Error: exit takes no arguments." << std::endl;
                return;
            }

            std::cout << "Exiting...\n";
            running = false;
        }},
    };
}

void Bootstrap::run() {
    std::string cmd;

    std::cout << "\033[2J\033[H";
    display();

    while (running) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, cmd);
        executeCommand(cmd);
    }
}

bool Bootstrap::isInitialized() {
    return initialized;
}

void Bootstrap::display() {
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

void Bootstrap::executeCommand(const std::string& cmd) {
    std::istringstream iss(cmd);
    std::vector<std::string> tokens;
    std::string word;

    while (iss >> word) {
        tokens.push_back(word);
    }

    if (tokens.empty()) return;

    std::string cmdToken = tokens[0];

    if (cmdToken != "initialize" && cmdToken != "exit") {
        std::cerr << "Error: Please run 'initialize' first.\n";
        return;
    }

    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    auto it = commands.find(cmdToken);
    if (it != commands.end()) {
        it->second(args);
    } else {
        std::cerr << "Error: Command \'" << cmdToken << "\' not recognized." << std::endl;
    }
}

void Bootstrap::initialize() {
    // Look for config.txt relative to wherever the program is launched from.
    const std::vector<std::string> configPaths = { "config.txt", "src/config.txt" };

    std::ifstream file;
    for (const auto& path : configPaths) {
        file.open(path);
        if (file.is_open()) break;
    }

    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open config.txt\n";
        return;
    }

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

    initialized = true;
    running = false;
}
