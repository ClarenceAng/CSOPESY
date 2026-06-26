#include "IDeclare.h"

IDeclare::IDeclare(uint16_t& var, uint16_t val)
    : var(var), val(val) {}

uint64_t IDeclare::execute() {
    var = val;
    return 1;
}