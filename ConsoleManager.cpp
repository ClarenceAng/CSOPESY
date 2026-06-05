#include "ConsoleManager.h"
#include "Console.h"

ConsoleManager::ConsoleManager() {
    running = true;

    consoleTable[MAIN_CONSOLE] = std::make_shared<MainConsole>();
    consoleTable[MARQUEE_CONSOLE] = std::make_shared<MarqueeConsole>();

    currentConsole = consoleTable[MAIN_CONSOLE];
}

ConsoleManager* ConsoleManager::singleton = nullptr;

void ConsoleManager::initialize() {
    singleton = new ConsoleManager();
}

ConsoleManager* ConsoleManager::getInstance() {
    return singleton;
}

void ConsoleManager::destroy() {
    delete singleton;
}

void ConsoleManager::start() {
    consoleThread = std::thread(&ConsoleManager::run, this);
}

void ConsoleManager::stop() {
    if (consoleThread.joinable()) {
        consoleThread.join();
    }
}

void ConsoleManager::run() {
    while (running) {
        currentConsole->run();
    }
}

bool ConsoleManager::isRunning() {
    return running;
}

void ConsoleManager::switchConsole(ConsoleType console) {
    currentConsole = consoleTable[console];
}

void ConsoleManager::terminate() {
    running = false;
}