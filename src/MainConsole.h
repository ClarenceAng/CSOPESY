#pragma once

#include <sstream>
#include <fstream>

#include "globals.h"
#include "Console.h"
#include "Scheduler.h"

using CommandMap = std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>>;

class MainConsole : public Console {
    public:
        MainConsole();
        ~MainConsole() = default;

        void run() override;
        void setRunning(bool running) override;

    private:
        void executeCommand(const std::string& cmd) override;

        bool running = false;
        CommandMap commands;
};