#pragma once

#include "Instruction.h"

class ISubtract : public Instruction {
    public:
        ISubtract(uint16_t& var, Operand op1, Operand val2);

        void execute() override;

    private:
        uint16_t& var;
        Operand op1;
        Operand op2;
};
