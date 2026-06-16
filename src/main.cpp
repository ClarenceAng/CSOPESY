#include "globals.h"
#include "Bootstrap.h"
#include "ConsoleManager.h"
#include "Scheduler.h"

int main() {
    Bootstrap bootstrap;
    bootstrap.run();

    if (!bootstrap.isInitialized()) {
        std::cout << "Program stopped successfully.\n";
        return 0;
    }

    bool running = true;

    ConsoleManager::getInstance()->start();

    while (running) {
        running = ConsoleManager::getInstance()->isRunning();
    }

    ConsoleManager::getInstance()->stop();
    Scheduler::getInstance()->stop();
    ConsoleManager::destroy();
    Scheduler::destroy();

    std::cout << "Program stopped successfully.\n";

    return 0;
};
