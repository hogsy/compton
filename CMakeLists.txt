cmake_minimum_required(VERSION 3.6)

# Set all of our output directories.
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/lib/")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

add_subdirectory(platform/platform/)
add_subdirectory(src/tools/pkgman/)

project(VCritters)

set(CMAKE_CXX_STANDARD 11)
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
        src/shared.h

        src/engine/*.cpp
        src/engine/GUI/*.cpp
        src/engine/*.h

        src/game/*.cpp
        src/game/Entities/*.cpp
        src/game/*.h
        )

add_executable(VCritters ${SOURCE_FILES}
        # engine
        src/engine/SpriteSheet.cpp
        src/engine/SpriteSheet.h
        src/engine/SimGame.h
        src/engine/JsonReader.cpp
        src/engine/JsonReader.h
        src/engine/ScriptParser.cpp
        src/engine/ScriptParser.h
        src/engine/FileSystem.cpp
        src/engine/LoaderGfx.cpp
        src/engine/LoaderPkg.cpp
        src/engine/Serializer.cpp
        src/engine/Serializer.h
        src/engine/vm/vm.c
        src/engine/PlayerManager.cpp
        src/engine/Random.cpp
        src/engine/Random.h
        src/engine/Camera.h
        src/engine/Timer.h

        # miniz
        src/3rdparty/miniz/miniz.c
        src/3rdparty/miniz/miniz.h

        # duktape
        src/3rdparty/duktape-2.2.0/duktape.c

        src/game/Entities/BaseCreature.cpp
        src/game/Entities/BaseCreature.h
        src/game/Entities/AIBrain.cpp
        src/game/Entities/AIBrain.h
        src/game/Entities/Tree.cpp
        src/game/Entities/Tree.h
        src/game/Utility.cpp
        src/game/Utility.h src/game/Entities/DebugWaypoint.cpp)

set_target_properties(VCritters PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

target_include_directories(VCritters PRIVATE
        src/engine/
        src/game/
        src/game/desktop/

        # 3rd party libraries
        src/3rdparty/
        src/3rdparty/miniz/
        src/3rdparty/duktape-2.2.0/
        )
if (WIN32)
    target_include_directories(VCritters PRIVATE src/3rdparty/allegro/include/)
    target_link_directories(VCritters PRIVATE src/3rdparty/allegro/lib/)
    target_link_libraries(VCritters -Wl,-Bstatic stdc++ winpthread -Wl,-Bdynamic -static-libstdc++ -static-libgcc)
endif (WIN32)
target_link_libraries(VCritters
        platform
        allegro
        allegro_dialog
        allegro_font
        allegro_ttf
        allegro_primitives
        allegro_audio
        allegro_acodec
        )