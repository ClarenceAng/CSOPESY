#include "ConsoleManager.h"

int main() {
    ConsoleManager::initialize();
    gl_cpuCycles = 0;
    gl_processCount = 0;
    bool running = true;

    ConsoleManager::getInstance()->start();

    while (running) {
        running = ConsoleManager::getInstance()->isRunning();
        
        gl_cpuCycles++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ConsoleManager::getInstance()->stop();
    ConsoleManager::destroy();

    std::cout << "CPU Cycles elapsed: " << gl_cpuCycles << std::endl;
    std::cout << "Program stopped successfully.\n";

    return 0;
};