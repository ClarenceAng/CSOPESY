#include "MainConsole.h"
#include "ConsoleManager.h"
#include "globals.h"

MainConsole::MainConsole() {
    commands = {
        { "screen", [this](const auto& args) {
            if (args.empty()) {
                std::cerr << "Error: Please provide arguments for screen." << std::endl;
                return;
            }

            if (args[0] == "-s") {
                if (args.size() != 2) {
                    std::cerr << "Error: Invalid arguments for screen -s." << std::endl;
                    return;
                }

                Scheduler::getInstance()->generateSingleProcess(args[1]);
                
            } else if (args[0] == "-r") {
                if (args.size() != 2) {
                    std::cerr << "Error: Invalid arguments for screen -r." << std::endl;
                    return;
                }

                ConsoleManager::getInstance()->switchConsole(args[1]);
                
            } else if (args[0] == "-ls") {
                if (args.size() != 1) {
                    std::cerr << "Error: Invalid arguments for screen -ls." << std::endl;
                    return;
                }

                Scheduler::getInstance()->getCpuUtilization(false);
            }
        }},

        { "scheduler-start", [this](const auto& args) { 
            if (!args.empty()) {
                std::cerr << "Error: scheduler-start takes no arguments." << std::endl;
                return;
            }

            std::thread(&Scheduler::generateMultipleProcesses, Scheduler::getInstance()).detach();
            std::cout << "Process generator starting." << std::endl;
        }},

        { "scheduler-stop", [this](const auto& args) { 
            if (!args.empty()) {
                std::cerr << "Error: scheduler-stop takes no arguments." << std::endl;
                return;
            }
            
            Scheduler::getInstance()->stopGenerator();
            std::cout << "Process generator stopped." << std::endl;
        }},

        { "report-util", [this](const auto& args) { 
            if (!args.empty()) {
                std::cerr << "Error: report-util takes no arguments." << std::endl;
                return;
            }
            
            Scheduler::getInstance()->getCpuUtilization(true);
        }},

        { "clear", [this](const auto& args) { 
            if (!args.empty()) {
                std::cerr << "Error: clear takes no arguments." << std::endl;
                return;
            }
            
            std::cout << "\033[2J\033[H"; headerDisplay(); 
        }},

        { "exit", [this](const auto& args) {
            if (!args.empty()) {
                std::cerr << "Error: exit takes no arguments." << std::endl;
                return;
            }
            
            std::cout << "Exiting...\n";
            ConsoleManager::getInstance()->terminate(); 
            running = false; 
        }},
    };
}

void MainConsole::run() {
    std::string cmd;

    while(running) {
        std::cout << "Enter a command: ";
        std::getline(std::cin, cmd);
        executeCommand(cmd);
    }
}

void MainConsole::setRunning(bool running) {
    this->running = running;
}

void MainConsole::executeCommand(const std::string& cmd) {
    std::istringstream iss(cmd);
    std::vector<std::string> tokens;
    std::string word;

    while (iss >> word) {
        tokens.push_back(word);
    }

    if (tokens.empty()) return;

    std::string cmdToken = tokens[0];

    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    auto it = commands.find(cmdToken);
    if (it != commands.end()) {
        it->second(args);
    } else {
        std::cerr << "Error: Command \'" << cmdToken << "\' not recognized." << std::endl;
    }
}