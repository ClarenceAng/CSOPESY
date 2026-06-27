#pragma once

#include "Instruction.h"

class ISleep : public Instruction {
    public:
        ISleep(uint8_t ticks);

        uint64_t execute() override;
        bool isLooping() override;

    private:
        uint8_t counter = 0;
        uint8_t ticks;
};