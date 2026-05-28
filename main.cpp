#include "ConsoleManager.h"

int main() {
    ConsoleManager::initialize();
    bool running = true;

    while (running) {
        ConsoleManager::getInstance()->run();

        running = ConsoleManager::getInstance()->isRunning();
    }

    ConsoleManager::destroy();

    std::cout << "Program stopped successfully.\n";

    return 0;
}