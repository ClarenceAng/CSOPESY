#pragma once

#include "Console.h"

class MainConsole : public Console {
    public:
        MainConsole();
        ~MainConsole() = default;

        void run() override;
    private:
        void executeCommand(std::string cmd) override;
        void display();
        
        bool running = false;
};