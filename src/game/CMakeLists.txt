project(game)

add_library(game STATIC
        ConfigLoader.cpp
        GameMode.cpp
        Terrain.cpp
        Utility.cpp

        Entities/AIBrain.cpp
        Entities/BaseBuilding.cpp
        Entities/BaseCharacter.cpp
        Entities/BaseCreature.cpp
        Entities/DebugWaypoint.cpp
        Entities/House.cpp
        Entities/StoreHouse.cpp
        Entities/Tree.cpp)

target_include_directories(game PRIVATE ../engine/)
target_link_libraries(game plcore)
