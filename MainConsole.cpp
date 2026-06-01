#include "MainConsole.h"
#include "ConsoleManager.h"
#include <sstream>
#include <fstream>

// Comment out the next line to disable the "initialize must run first" gate (for testing).
#define REQUIRE_INITIALIZE

    MainConsole::MainConsole() {}

void MainConsole::run() {
    std::string cmd;
    
    std::cout << "\033[2J\033[H";
    display();
    running = true; 

    while(running) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, cmd);
        executeCommand(cmd);
    }
}

void MainConsole::display() {
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
              << "Last updated: 06-01-2026                      " << std::endl
              << "----------------------------------------------" << std::endl              
              ;
}

void MainConsole::executeCommand(std::string cmd) {
    std::unordered_map<std::string, std::function<void()>> command_list = {
        { "initialize", [this](){ MainConsole::initialize(); } },
        { "screen", [](){ std::cout << "screen command recognized. Doing something.\n"; } },
        { "scheduler-start", [](){ std::cout << "scheduler-start command recognized. Doing something.\n"; } },
        { "scheduler-stop", [](){ std::cout << "scheduler-stop command recognized. Doing something.\n"; } },
        { "report-util", [](){ std::cout << "report-util command recognized. Doing something.\n"; } },
        { "clear", [this](){ std::cout << "\033[2J\033[H"; display(); } },
        { "exit", [this](){ std::cout << "Exiting...\n"; ConsoleManager::getInstance()->terminate(); running = false; } },
        { "marquee", [this]() { ConsoleManager::getInstance()->switchConsole(MARQUEE_CONSOLE); running = false; }}
    };

#ifdef REQUIRE_INITIALIZE
    if (!initializeFlag && cmd != "initialize" && cmd != "exit") {
        std::cout << "Please run 'initialize' first.\n";
        return;
    }
#endif

    auto it = command_list.find(cmd);

    if (it == command_list.end()) {
        std::cerr << "\'" << cmd << "\' command not found.\n";
    } else {
        command_list[cmd]();
    }
}

void MainConsole::initialize() {
    std::ifstream file("config.txt");
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open config.txt\n";
        return;
    }

    keys.clear();
    values.clear();

    while (std::getline(file, line)) {
        std::stringstream ss(line);

        std::string key, value;

        if (ss >> key >> value) {
            keys.push_back(key);
            values.push_back(value);
        }
    }

    initializeFlag = true;

    std::cout << "Configuration:\n";
    for (size_t i = 0; i < keys.size(); i++) {
        std::cout << keys[i] << " = " << values[i] << '\n';
    }
}