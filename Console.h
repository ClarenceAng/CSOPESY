#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <chrono>

class Console {
    public:
        virtual ~Console() = default;

        virtual void run() = 0;
    private:
        virtual void executeCommand(std::string cmd) = 0;
};