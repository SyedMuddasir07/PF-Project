#include "logger.h"
#include "util.h"
#include "level.h"

#include <filesystem>
#include <fstream>

// File persistence for savegame.txt and game_summary.txt using primitive variables.
bool saveGameStateToFile(const char* filename,
                         int playerFloor, int playerRoom,
                         int energy, int time, int score,
                         const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                         const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft,
                         const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                         const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    if (filename == nullptr || filename[0] == '\0') {
        return false;
    }

    const std::filesystem::path filePath(filename);
    const std::filesystem::path parentPath = filePath.parent_path();

    std::error_code dirError;
    if (!parentPath.empty()) {
        std::filesystem::create_directories(parentPath, dirError);
        if (dirError) {
            return false;
        }
    }

    std::ofstream output(filePath);
    if (!output) {
        return false;
    }

    output << playerFloor << ' ' << playerRoom << '\n'
           << energy << ' ' << time << ' ' << score << '\n'
           << inventoryCount << '\n';

    for (int i = 0; i < inventoryCount && i < MAX_INVENTORY_CAPACITY; ++i) {
        output << inventory[i] << '\n';
    }

    for (int i = 0; i < NUM_OBJECTIVES; ++i) {
        output << (objectives[i] ? 1 : 0) << '\n';
    }

    output << emergencyTripsLeft << '\n';

    for (int floor = 0; floor < NUM_FLOORS; ++floor) {
        for (int room = 0; room < ROOMS_PER_FLOOR; ++room) {
            int count = entityCounts[floor][room];
            if (count < 0) {
                count = 0;
            } else if (count > MAX_ENTITIES_PER_ROOM) {
                count = MAX_ENTITIES_PER_ROOM;
            }

            output << count;
            for (int entity = 0; entity < count; ++entity) {
                output << ' ' << roomEntities[floor][room][entity];
            }
            output << '\n';
        }
    }

    output.close();
    return output.good();
}

