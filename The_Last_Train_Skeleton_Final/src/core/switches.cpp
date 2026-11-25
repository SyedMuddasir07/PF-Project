#include "switches.h"
#include "util.h"

// Door lock mechanics, lifts, and cabinet verification without structures
bool canUseLift1(int fromFloor, int toFloor)
{
    // Lift1 only connects floors 1 and 2, either direction.
    return (fromFloor == 1 && toFloor == 2) || (fromFloor == 2 && toFloor == 1);
}
