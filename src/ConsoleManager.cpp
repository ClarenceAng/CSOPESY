#include "ConsoleManager.h"
#include "Console.h"

ConsoleManager::ConsoleManager() {
    running = true;

    consoleTable[MAIN_CONSOLE] = std::make_shared<MainConsole>();

    currentConsole = consoleTable[MAIN_CONSOLE];
    currentConsole->setRunning(true);
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

bool ConsoleManager::registerScreen(std::string consoleName, bool isVerbose) {
    std::lock_guard<std::mutex> lock(screen_mutex);
    if (consoleTable.contains(consoleName)) {
        if (isVerbose) {
            std::cerr << "Error: Screen \"" << consoleName << "\" already exists." << std::endl;
        }
        return false;
    }

    consoleTable[consoleName] = std::make_shared<Screen>(consoleName);

    return true;
}

void ConsoleManager::switchConsole(std::string consoleName) {
    std::lock_guard<std::mutex> lock(screen_mutex);
    if (!consoleTable.contains(consoleName)) {
        std::cerr << "Error: Screen \"" << consoleName << "\" not registered." << std::endl;
        return;
    }

    if (consoleName != MAIN_CONSOLE && consoleTable[consoleName]->isProcessFinished()) {
        std::cerr << "Error: Screen \"" << consoleName << "\" has finished its process." << std::endl;
        return;
    }

    currentConsole->setRunning(false);
    currentConsole = consoleTable[consoleName];

    if (consoleName == MAIN_CONSOLE) {
        std::cout << "\033[2J\033[H"; 
        headerDisplay(); 
    }

    currentConsole->setRunning(true);
}

void ConsoleManager::setScreenProcess(std::shared_ptr<Process> process) {
    std::lock_guard<std::mutex> lock(screen_mutex);
    
    auto it = consoleTable.find(process->getProcessName());
    if (it != consoleTable.end()) {
        it->second->setProcess(process);
    } else {
        std::cerr << "Error: Screen not found for process "
                  << process->getProcessName() << std::endl;
    }
}

void ConsoleManager::terminate() {
    running = false;
}