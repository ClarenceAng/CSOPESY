#pragma once

#include "Instruction.h"

class ISubtract : public Instruction {
    public:
        ISubtract(uint16_t& var, uint16_t& val1, uint16_t& val2);
        ISubtract(uint16_t& var, uint16_t lit1, uint16_t lit2);

        void execute() override;

    private:
        uint16_t& var;
        uint16_t litStore1{};
        uint16_t litStore2{};
        uint16_t* pVal1;
        uint16_t* pVal2;
};
