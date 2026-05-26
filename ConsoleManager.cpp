#include "ConsoleManager.h"

ConsoleManager::ConsoleManager() {
    consoleTable[MAIN_CONSOLE] = std::make_shared<MainConsole>();

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

void ConsoleManager::run() {
    currentConsole->process();
}