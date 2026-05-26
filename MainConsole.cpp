#include "MainConsole.h"

MainConsole::MainConsole() { running = true; }

void MainConsole::display() {
    // ASCII Art Generator: https://patorjk.com/software/taag/
    std::cout << R"( _____  _____  ___________ _____ _______   __)" << std::endl
              << R"(/  __ \/  ___||  _  | ___ \  ___/  ___\ \ / /)" << std::endl
              << R"(| /  \/\ `--. | | | | |_/ / |__ \ `--. \ V /)" << std::endl
              << R"(| |     `--. \| | | |  __/|  __| `--. \ \ /)" << std::endl
              << R"(| \__/\/\__/ /\ \_/ / |   | |___/\__/ / | |)" << std::endl
              << R"( \____/\____/  \___/\_|   \____/\____/  \_/)" << std::endl;
}

void MainConsole::process() {
    std::string cmd;

    display();

    while(running) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, cmd);
        executeCommand(cmd);
    }
}

void MainConsole::executeCommand(std::string cmd) {
    std::unordered_map<std::string, std::function<void()>> command_list = {
        { "initialize", [](){ std::cout << "initialize command recognized. Doing something.\n"; } },
        { "screen", [](){ std::cout << "screen command recognized. Doing something.\n"; } },
        { "scheduler-start", [](){ std::cout << "scheduler-start command recognized. Doing something.\n"; } },
        { "scheduler-stop", [](){ std::cout << "scheduler-stop command recognized. Doing something.\n"; } },
        { "report-util", [](){ std::cout << "report-util command recognized. Doing something.\n"; } },
        { "clear", [this](){ std::cout << "\033[2J\033[H"; display(); } },
        { "exit", [this](){ std::cout << "Exiting...\n"; running = false; } },
    };

    auto it = command_list.find(cmd);

    if (it == command_list.end()) {
        std::cerr << "\'" << cmd << "\' command not found.\n";
    } else {
        command_list[cmd]();
    }
}