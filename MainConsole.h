#pragma once

#include "Console.h"

class MainConsole : public Console {
    public:
        MainConsole();
        ~MainConsole() = default;

        void display() override;
        void process() override;
    private:
        void executeCommand(std::string cmd) override;
        
        bool running = false;
};