#pragma once

#include "Console.h"

class MainConsole : public Console {
    public:
        MainConsole();
        ~MainConsole() = default;

        void run() override;
    private:
        bool initializeFlag = false;
        std::unordered_map<std::string, std::string> config;

        void executeCommand(std::string cmd) override;
        void display();
        void initialize();
        
        bool running = false;
};