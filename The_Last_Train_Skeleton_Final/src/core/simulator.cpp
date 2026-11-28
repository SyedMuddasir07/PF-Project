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

bool handleInspectCommand(int currentFloor, int currentRoom,
                          int &energy, int &time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int &score)
{

    renderRoomDetails(currentFloor, currentRoom, roomEntities, entityCount); // 1. Show what is in the room

    for (int i = 0; i < entityCount; ++i)
    {
        char symbol = roomEntities[currentFloor - 1][currentRoom - 1][i];

        if (symbol == ENT_CLUE_4 && !objectives[OBJ_CLUE_4])
        {
            objectives[OBJ_CLUE_4] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
        }
        else if (symbol == ENT_CLUE_2 && !objectives[OBJ_CLUE_2])
        {
            objectives[OBJ_CLUE_2] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
        }
        else if (symbol == ENT_CLUE_7 && !objectives[OBJ_CLUE_7])
        {
            objectives[OBJ_CLUE_7] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
        }
    }
    applyResourceDelta(energy, time, COST_INSPECT_ENERGY, COST_INSPECT_TIME);
    return true;
}

bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int &energy, int &time, int &score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int &entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int &inventoryCount,
                          bool objectives[NUM_OBJECTIVES])
{

    char *items = roomEntities[currentFloor - 1][currentRoom - 1]; // The items of the current room

    int index = -1; // it Find the item in the room
    for (int i = 0; i < entityCount; ++i)
    {
        if (items[i] == itemSymbol)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        return false;
    if (itemSymbol == ENT_CLUE_4 || itemSymbol == ENT_CLUE_2 || itemSymbol == ENT_CLUE_7)
        return false;
    if (itemSymbol == ENT_CABINET_TRAIN_KEY && !objectives[OBJ_CABINET_UNLOCKED])
        return false;

    if (!addItem(inventory, inventoryCount, itemSymbol))
        return false;

    for (int i = index + 1; i < entityCount; ++i)
        items[i - 1] = items[i];
    entityCount--;
    items[entityCount] = ENT_NONE;

    if (itemSymbol == ENT_MASTER_KEY && !objectives[OBJ_MASTER_KEY])
    { // objectives[OBJ_MASTER_KEY] if false then true
        objectives[OBJ_MASTER_KEY] = true;
        applyScoreDelta(score, SCORE_KEY_COLLECT);
    }
    else if (itemSymbol == ENT_FUSE)
    {
        objectives[OBJ_FUSE_COLLECTED] = true;
    }
    else if (itemSymbol == ENT_CAT_CARRIER)
    {
        objectives[OBJ_CAT_CARRIER] = true;
    }
    else if (itemSymbol == ENT_CABINET_TRAIN_KEY && !objectives[OBJ_TRAIN_KEY])
    {
        objectives[OBJ_TRAIN_KEY] = true;
        applyScoreDelta(score, SCORE_TRAIN_KEY);
    }
    else if (itemSymbol == ENT_LIFT_PASS)
    {
        if (!objectives[OBJ_LIFT_PASS])
        {
            objectives[OBJ_LIFT_PASS] = true;
            applyScoreDelta(score, SCORE_LIFT_PASS);
        }
        objectives[OBJ_LIFT2_ACTIVE] = objectives[OBJ_FUSE_INSTALLED];
    }
    applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);
    return true;
}