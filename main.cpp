#include "ConsoleManager.h"

int main() {
    ConsoleManager::initialize();

    ConsoleManager::getInstance()->run();

    return 0;
}