#include "ISubtract.h"

ISubtract::ISubtract(uint16_t& var, uint16_t& val1, uint16_t& val2)
    : var(var), pVal1(&val1), pVal2(&val2) {}

ISubtract::ISubtract(uint16_t& var, uint16_t lit1, uint16_t lit2)
    : var(var), litStore1(lit1), litStore2(lit2), pVal1(&litStore1), pVal2(&litStore2) {}

void ISubtract::execute() {
    var = (*pVal2 > *pVal1) ? 0 : static_cast<uint16_t>(*pVal1 - *pVal2);
}
