#pragma once

#include "globals.h"

struct Operand {
    bool isLiteral;
    uint16_t value;
    uint16_t* reference;

    uint16_t get() const {
        return isLiteral ? value : *reference;
    }
};

class Instruction {
    public:
        ~Instruction() = default;
        
        virtual uint64_t execute() = 0;
        virtual bool isLooping() {
            return 0;
        }
};