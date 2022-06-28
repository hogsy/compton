project(game)

add_library(game STATIC
        DSGameMode.cpp
        Terrain.cpp
        Utility.cpp

        Entities/ai/Brain.cpp
        Entities/ai/Sensor.cpp

        Entities/BaseBuilding.cpp
        Entities/BaseCharacter.cpp
        Entities/BaseCreature.cpp
        Entities/DebugWaypoint.cpp
        Entities/House.cpp
        Entities/StoreHouse.cpp
        Entities/Tree.cpp)

target_include_directories(game PRIVATE ../engine/)
