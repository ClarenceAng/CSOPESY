#pragma once

#include "Console.h"

class MainConsole : public Console {
    public:
        MainConsole();
        ~MainConsole() = default;

        void run() override;
    private:
        bool initializeFlag = false;
        std::vector<std::string> keys;
        std::vector<std::string> values;

        void executeCommand(std::string cmd) override;
        void display();
        void initialize();
        
        bool running = false;
};