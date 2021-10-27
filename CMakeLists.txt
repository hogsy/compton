cmake_minimum_required(VERSION 3.6)

# Set all of our output directories.
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/lib/")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

add_subdirectory(src/hei/plcore/)
add_subdirectory(src/hei/plgraphics/)

add_subdirectory(src/tools/pkgman/)

#add_subdirectory(src/game/)

project(Compton)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Get the current working branch
execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the latest abbreviated commit hash of the working branch
execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Get the number of commits on the working branch
execute_process(
        COMMAND git rev-list HEAD --count
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_COUNT
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions(-DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}")
add_definitions(-DGIT_COMMIT_COUNT="${GIT_COMMIT_COUNT}")
add_definitions(-DGIT_BRANCH="${GIT_BRANCH}")

file(GLOB SOURCE_FILES
        src/engine/GUI/*.cpp
        src/game/Entities/*.cpp
        )

add_executable(Compton ${SOURCE_FILES}
        # engine
        src/engine/App.cpp
        src/engine/BitmapFont.cpp
        src/engine/Entity.cpp
        src/engine/EntityManager.cpp
        src/engine/FileSystem.cpp
        src/engine/ImageManager.cpp
        src/engine/JsonReader.cpp
        src/engine/LoaderPkg.cpp
        src/engine/PlayerManager.cpp
        src/engine/Random.cpp
        src/engine/ScriptParser.cpp
        src/engine/Serializer.cpp
        src/engine/SpriteSheet.cpp

        # game
        src/game/Entities/ai/Brain.cpp
        src/game/Entities/ai/Sensor.cpp
        src/game/Entities/BaseCreature.cpp
        src/game/Entities/Boid.cpp
        src/game/Entities/Tree.cpp
        src/game/Entities/DebugWaypoint.cpp
        src/game/Background.cpp
        src/game/ConfigLoader.cpp
        src/game/GameMode.cpp
        src/game/Terrain.cpp
        src/game/Utility.cpp

        # duktape
        src/3rdparty/duktape-2.2.0/src/duktape.c
        )

set_target_properties(Compton PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

target_include_directories(Compton PRIVATE
        src/engine/
        src/game/

        # 3rd party libraries
        src/3rdparty/
        src/3rdparty/miniz/
        src/3rdparty/duktape-2.2.0/src/
        )
if (WIN32)
    target_include_directories(Compton PRIVATE src/3rdparty/allegro/include/)
    target_link_directories(Compton PRIVATE src/3rdparty/allegro/lib/)
    target_link_libraries(Compton -Wl,-Bstatic stdc++ winpthread -Wl,-Bdynamic -static-libstdc++ -static-libgcc)
endif (WIN32)
target_link_libraries(Compton
        plcore
        plgraphics

        allegro
        allegro_dialog
        allegro_font
        allegro_ttf
        allegro_primitives
        allegro_audio
        allegro_acodec
        )