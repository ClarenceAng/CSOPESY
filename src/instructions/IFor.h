#pragma once

#include "Instruction.h"

using ForLoop = std::vector<std::unique_ptr<Instruction>>;

class IFor : public Instruction {
    public:
        IFor(std::unique_ptr<ForLoop> instructions, uint16_t repeats);

        uint64_t execute() override;
        bool isLooping() override;

    private:
        std::unique_ptr<ForLoop> instructions;
        uint16_t repeats;
        uint16_t repeatCount = 0;

        uint64_t instructionSize;
        uint64_t forIndex = 0;
        bool isRunning = true;;
};