#include "IAdd.h"

IAdd::IAdd(uint16_t& var, uint16_t val1, uint16_t val2)
    : var(var), val1(val1), val2(val2) {}

void IAdd::execute() {
    uint32_t result = static_cast<uint16_t>(val1 + val2);
    var = static_cast<uint16_t>(std::min(result, static_cast<uint32_t>(65535)));
}
