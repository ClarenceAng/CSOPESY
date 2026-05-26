#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <memory>

#include "Console.h"
#include "MainConsole.h"

enum ConsoleType {
    MAIN_CONSOLE,
    MARQUEE_CONSOLE
};

using ConsoleTable = std::unordered_map<ConsoleType, std::shared_ptr<Console>>;

class ConsoleManager {
    public:
        static void initialize();
        static ConsoleManager* getInstance();
        static void destroy();
    
        void run();

    private:
        ConsoleManager();
        ~ConsoleManager() = default;

        static ConsoleManager* singleton;
        ConsoleTable consoleTable;
        std::shared_ptr<Console> currentConsole;
};
