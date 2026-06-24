#include "ISubtract.h"

ISubtract::ISubtract(uint16_t& var, Operand op1, Operand op2)
    : var(var), op1(op1), op2(op2) {}

void ISubtract::execute() {
    var = (op2.get() > op1.get()) ? 0 : op1.get() - op2.get();
}
