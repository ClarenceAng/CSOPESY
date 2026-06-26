#include "IFor.h"

IFor::IFor(std::unique_ptr<ForLoop> instructions, uint16_t repeats)
    : instructions(std::move(instructions)), repeats(repeats) 
{
    instructionSize = this->instructions->size();
}

uint64_t IFor::execute() {
    isRunning = true;
    uint64_t completed = 0;

    if (instructionSize > 0) {
        completed = (*instructions)[forIndex]->execute();   // one leaf (or 0 if inner SLEEP busy-waits)

        if (!(*instructions)[forIndex]->isLooping()) {
            forIndex++;
        }
    }

    if (forIndex == instructionSize) {
        forIndex = 0;
        repeatCount++;
    }

    if (repeatCount == repeats) {
        isRunning = false;
        repeatCount = 0;
    }

    return completed;
}

bool IFor::isLooping() {
    return isRunning;
}