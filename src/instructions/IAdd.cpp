#include "IAdd.h"

IAdd::IAdd(uint16_t& var, Operand op1, Operand op2)
    : var(var), op1(op1), op2(op2) {}

uint64_t IAdd::execute() {
    uint32_t result = static_cast<uint32_t>(op1.get()) + static_cast<uint32_t>(op2.get());
    var = static_cast<uint16_t>(std::min(result, static_cast<uint32_t>(65535)));
    return 1;
}