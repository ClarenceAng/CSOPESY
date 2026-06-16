#pragma once

#include <sstream>
#include <fstream>

#include "globals.h"

using CommandMap = std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>>;

class Bootstrap {
    public:
        Bootstrap();
        ~Bootstrap() = default;

        void run();
        bool isInitialized();

    private:
        void executeCommand(const std::string& cmd);
        void display();
        void initialize();

        bool initialized = false;
        bool running = true;
        CommandMap commands;
};
