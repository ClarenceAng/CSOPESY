#include "ISleep.h"

ISleep::ISleep(uint8_t ticks) : ticks(ticks) {}

uint64_t ISleep::execute() {
    counter++;
    if (counter >= ticks) {
        counter = 0;   // reset so the SLEEP works again on the next loop iteration
        return 1;      // the SLEEP instruction completes on this tick
    }
    return 0;          // still busy-waiting; no leaf completed
}

bool ISleep::isLooping() {
    return counter != 0;   // busy-waiting iff started (counter > 0) but not yet completed
}

