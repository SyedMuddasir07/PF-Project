#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"

// Modular simulation loop using primitive variables

bool handleMoveCommand(int targetFloor, int targetRoom,
                       int &currentFloor, int &currentRoom,
                       int &energy, int &time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int &emergencyTripsLeft, bool lift2Operational)
{

    char connection;
    if (!isDirectlyConnected(currentFloor, currentRoom, targetFloor, targetRoom,
                             horizontalConn, connection))
        return false;
    if (!canTraverse(targetFloor, targetRoom, connection, inventory, inventoryCount,
                     emergencyTripsLeft, lift2Operational))
        return false;
    int deltaEnergy = COST_MOVE_ORDINARY_ENERGY;
    int deltaTime = COST_MOVE_ORDINARY_TIME;
    if (connection == CONN_WINDOW)
    {
        deltaEnergy = COST_MOVE_WINDOW_ENERGY;
        deltaTime = COST_MOVE_WINDOW_TIME;
    }
    else if (connection == '1' || connection == '2')
    {
        deltaEnergy = COST_MOVE_LIFT_ENERGY;
        deltaTime = COST_MOVE_LIFT_TIME;
    }
    if (connection == '2' && !lift2Operational)
        emergencyTripsLeft--;
    currentFloor = targetFloor;
    currentRoom = targetRoom;
    applyResourceDelta(energy, time, deltaEnergy, deltaTime);
    return true;
}
