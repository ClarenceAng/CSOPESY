#include "IAdd.h"
IAdd::IAdd(uint16_t& var, uint16_t& val1, uint16_t& val2)
    : var(var), pVal1(&val1), pVal2(&val2) {}

IAdd::IAdd(uint16_t& var, uint16_t lit1, uint16_t lit2)
    : var(var), litStore1(lit1), litStore2(lit2), pVal1(&litStore1), pVal2(&litStore2) {}

void IAdd::execute() {
    uint32_t result = static_cast<uint32_t>(*pVal1) + static_cast<uint32_t>(*pVal2);
    var = static_cast<uint16_t>(std::min(result, static_cast<uint32_t>(65535)));
}
