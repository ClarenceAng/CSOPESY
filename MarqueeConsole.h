#pragma once

#include "Console.h"

struct ConsoleDimensions {
    int width;
    int height;
};

class MarqueeConsole : public Console {
    public:
        MarqueeConsole();
        ~MarqueeConsole() = default;

        void run() override;
    private:
        void executeCommand(std::string cmd) override;

        bool running = false;
        std::vector<std::string> consoleHistory;
        ConsoleDimensions currentDimensions;
};