#pragma once

#include "Instruction.h"

class IAdd : public Instruction {
    public:
        IAdd(uint16_t& var, Operand op1, Operand op2);

        uint64_t execute() override;

    private:
        uint16_t& var;
        Operand op1;
        Operand op2;
    };
