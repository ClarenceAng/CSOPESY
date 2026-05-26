#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

class Console {
    public:
        virtual ~Console() = default;

        virtual void display() = 0;
        virtual void process() = 0;
    private:
        virtual void executeCommand(std::string cmd) = 0;
};