#include "switches.h"
#include "util.h"

// Door lock mechanics, lifts, and cabinet verification without structures
bool canUseLift1(int fromFloor, int toFloor)
{
    // Lift1 only connects floors 1 and 2, either direction.
    return (fromFloor == 1 && toFloor == 2) || (fromFloor == 2 && toFloor == 1);
}
bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft)
{
    bool fromStop = fromFloor == 2 || fromFloor == 4 || fromFloor == 5; // Lift2 only stops at floors 2, 4, and 5
    bool toStop = toFloor == 2 || toFloor == 4 || toFloor == 5;
    // Lift 2 only works if the lift is powered (fuse installed and lift pass found) or the player still has an emergency trip.
    return fromStop && toStop && fromFloor != toFloor &&
           (lift2Operational || emergencyTripsLeft > 0);
}
