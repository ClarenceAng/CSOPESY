#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <memory>

#include "Console.h"
#include "MainConsole.h"
#include "MarqueeConsole.h"

class Console;

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
        bool isRunning();

    private:
        ConsoleManager();
        ~ConsoleManager() = default;
        void switchConsole(ConsoleType console);
        void terminate();

        bool running = false;
        static ConsoleManager* singleton;
        ConsoleTable consoleTable;
        std::shared_ptr<Console> currentConsole;

        friend class MainConsole;
        friend class MarqueeConsole;
};
