#include "physics.h"
#include "util.h"
#include "switches.h"
#include <cmath>

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol)
{
    for (int i = 0; i < inventoryCount; i++)
    {
        if (inventory[i] == itemSymbol)
            return true;
    }
    return false;
}

bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int &inventoryCount, char itemSymbol)
{
    if (inventoryCount >= MAX_INVENTORY_CAPACITY)
        return false;
    inventory[inventoryCount] = itemSymbol;
    inventoryCount++;
    return true;
}
// We use `&` so the function changes the caller's real variable instead of a throwaway copy.
bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int &inventoryCount, char itemSymbol)
{
    for (int i = 0; i < inventoryCount; i++)
    {
        if (inventory[i] == itemSymbol)
        {
            for (int j = i; j < inventoryCount - 1; j++)
                inventory[j] = inventory[j + 1];
            inventoryCount--;
            return true;
        }
    }
    return false;
}

void applyResourceDelta(int &energy, int &time, int deltaEnergy, int deltaTime)
{
    energy = clampInt(energy + deltaEnergy, 0, MAX_ENERGY);
    time = clampInt(time + deltaTime, 0, MAX_TIME);
}

void applyScoreDelta(int &score, int deltaScore)
{
    score += deltaScore;
}


// Movement validation and resource adjustments without structures
bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char &outConnType)
{
    // start with no connection, because at if no connection is found, we will return false
    outConnType = CONN_NONE;
    // if any floor or room number is outside the building, stop
    if (fromF < 1 || fromF > NUM_FLOORS || toF < 1 || toF > NUM_FLOORS ||
        fromR < 1 || fromR > ROOMS_PER_FLOOR || toR < 1 || toR > ROOMS_PER_FLOOR)
    {
        return false;
    }
    // SAME floor, rooms side by side like R1 - R2
    if (fromF == toF && std::abs(fromR - toR) == 1)
    {
        // pick the smaller room number
        int left = fromR;
        if (toR < fromR)
            left = toR;
        // read the link type (door, window, locked door, wall) from the map
        outConnType = horizontalConn[fromF - 1][left - 1]; // row and column way
    }
    // SAME room number, different floor stairs or lifts
    else if (fromR == toR)
    {
        // pick the lower floor number
        int low = fromF;
        if (toF < fromF)
            low = toF;
        // Stairs--- only in room 1, floors one apart, between 1-2, 2-3 and 4-5
        if (fromR == 1 && std::abs(fromF - toF) == 1 &&
            (low == 1 || low == 2 || low == 4))
        {
            outConnType = 'S';
        }
        // Lift 1--- only in room 2 or 3, between floors 1 and 2
        if ((fromR == 2 || fromR == 3) && canUseLift1(fromF, toF))
        {
            outConnType = '1';
        }
        // Lift 2---- only in room 4, floors 2, 4 and 5
        // true, 0 here only checks the floors are valid; the power check happens later
        if (fromR == 4 && canUseLift2(fromF, toF, true, 0))
        {
            outConnType = '2';
        }
    }
    // return true only if we found a real link
    return outConnType != CONN_NONE && outConnType != CONN_SOLID_WALL;
}

bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational)
{
    if (connType == CONN_NONE || connType == CONN_SOLID_WALL)
        return false;

    if (connType == CONN_LOCKED_DOOR)
        return hasItem(inventory, inventoryCount, ENT_MASTER_KEY);

    if (connType == '2')
        return lift2Operational || emergencyTripsLeft > 0;

    return true; // door, window, stairs, lift 1
}