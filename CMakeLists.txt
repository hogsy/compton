cmake_minimum_required(VERSION 3.6)

project(Compton)

##############################################################
# Set all of our output directories.

include(DetermineTargetArchitecture.cmake)
determine_target_architecture(COMPTON_SYSTEM_ARCH)

if (UNIX)
    message("Assuming Unix - " ${COMPTON_SYSTEM_ARCH})
    set(COMPTON_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/runtime/unix-${COMPTON_SYSTEM_ARCH}/")
elseif (WIN32)
    message("Assuming Win32 - " ${COMPTON_SYSTEM_ARCH})
    set(COMPTON_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/runtime/win32-${COMPTON_SYSTEM_ARCH}/")
elseif (APPLE)
    message("Assuming Apple - " ${COMPTON_SYSTEM_ARCH})
    set(COMPTON_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/runtime/apple-${COMPTON_SYSTEM_ARCH}/")
else ()
    message("Unknown platform - " ${COMPTON_SYSTEM_ARCH})
    set(COMPTON_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/runtime/${COMPTON_SYSTEM_ARCH}/")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${COMPTON_OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${COMPTON_OUTPUT_DIR}")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${COMPTON_OUTPUT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${COMPTON_OUTPUT_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${COMPTON_OUTPUT_DIR})
endforeach (OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

##############################################################

add_subdirectory(src/hei/plcore/)
add_subdirectory(src/hei/plgraphics/)

add_subdirectory(src/lisp/)

add_subdirectory(src/tools/pkgman/)

#add_subdirectory(src/game/)

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
        )

set(DS_SOURCE_FILES
        src/game/DS/HumanCreature.cpp
        src/game/DS/Tree.cpp
        )

add_executable(Compton ${SOURCE_FILES}
        # engine
        src/engine/Renderer/DrawRoutines.cpp
        src/engine/Input/Action.cpp
        src/engine/Input/InputManager.cpp

        src/engine/App.cpp
        src/engine/BitmapFont.cpp
        src/engine/Entity.cpp
        src/engine/EntityManager.cpp
        src/engine/FileSystem.cpp
        src/engine/LispInterface.cpp
        src/engine/LoaderPkg.cpp
        src/engine/Random.cpp
        src/engine/Serializer.cpp
        src/engine/Sprite.cpp
        src/engine/SpriteAnimator.cpp
        src/engine/SpriteManager.cpp
        src/engine/SpriteSheet.cpp

        # game
        src/game/Entities/ai/Brain.cpp
        src/game/Entities/ai/Sensor.cpp
        src/game/Entities/BaseAnimated.cpp
        src/game/Entities/BaseBuilding.cpp
        src/game/Entities/BaseCharacter.cpp
        src/game/Entities/BaseCreature.cpp
        #src/game/Entities/Boid.cpp
        #src/game/Entities/BoidManager.cpp
        src/game/Entities/DebugWaypoint.cpp
        src/game/Background.cpp
        src/game/GameMode.cpp
        src/game/PlayerManager.cpp
        src/game/Terrain.cpp
        src/game/Utility.cpp
        src/game/World.cpp

        # DS specific files
        ${DS_SOURCE_FILES}
        )

target_include_directories(Compton PRIVATE
        src/engine/
        src/game/

        # 3rd party libraries
        src/3rdparty/
        src/3rdparty/miniz/
        )

if (WIN32)
    target_include_directories(Compton PRIVATE src/3rdparty/allegro/include/)
    target_link_directories(Compton PRIVATE src/3rdparty/allegro/lib/)
    target_link_libraries(Compton -Wl,-Bstatic stdc++ winpthread -Wl,-Bdynamic -static-libstdc++ -static-libgcc)
endif (WIN32)

# On Apple target, need to use pkgconfig to find allegro :(
if (APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALLEGRO REQUIRED allegro-5)
    target_include_directories(Compton PRIVATE ${ALLEGRO_INCLUDE_DIRS})
    target_link_directories(Compton PRIVATE ${ALLEGRO_LIBRARY_DIRS})
endif (APPLE)

target_link_libraries(Compton
        plcore
        plgraphics

        lisp

        allegro
        allegro_main
        allegro_dialog
        allegro_audio
        allegro_acodec
        )