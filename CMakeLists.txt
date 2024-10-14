cmake_minimum_required(VERSION 3.6)

# Set all of our output directories.
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/lib/")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

add_subdirectory(src/3rdparty/hei/plcore/)
add_subdirectory(src/3rdparty/hei/plgraphics/)

#add_subdirectory(src/game/)

project(Chorizo)

option(CHORIZO_BUILD_EDITOR "Build editor" ON)

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

include_directories(src/public)

add_subdirectory(src/lisp)

file(GLOB ENGINE_SOURCE_FILES
        src/engine/GUI/*.cpp
        src/engine/App.cpp
        src/engine/BitmapFont.cpp
        src/engine/Entity.cpp
        src/engine/EntityManager.cpp
        src/engine/FileSystem.cpp
        src/engine/ImageManager.cpp
        src/engine/PlayerManager.cpp
        src/engine/Random.cpp
        src/engine/render.cpp
        src/engine/script.cpp
        src/engine/ScriptParser.cpp
        src/engine/Serializer.cpp
        src/engine/SpriteSheet.cpp
)
file(GLOB GAME_SOURCE_FILES
        src/game/Entities/ai/Brain.cpp
        src/game/Entities/ai/Sensor.cpp
        src/game/Entities/BaseCharacter.cpp
        src/game/Entities/BaseCreature.cpp
        src/game/Entities/Boid.cpp
        src/game/Entities/BoidManager.cpp
        src/game/Entities/DebugWaypoint.cpp
        src/game/Background.cpp
        src/game/GameMode.cpp
)

####################################################################
# Main Executable

add_executable(chorizo WIN32
        ${ENGINE_SOURCE_FILES}
        ${GAME_SOURCE_FILES}
)

set_target_properties(chorizo PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

target_include_directories(chorizo PRIVATE
        src/engine/
        src/game/

        # 3rd party libraries
        src/3rdparty/
        src/3rdparty/miniz/
        src/3rdparty/duktape-2.2.0/src/
)
if (WIN32)
    target_include_directories(chorizo PRIVATE src/3rdparty/allegro/include/)
    target_link_directories(chorizo PRIVATE src/3rdparty/allegro/lib/)
    target_link_libraries(chorizo -Wl,-Bstatic stdc++ winpthread -Wl,-Bdynamic -static-libstdc++ -static-libgcc)
endif (WIN32)
target_link_libraries(chorizo
        plcore
        plgraphics

        allegro
        allegro_dialog
        allegro_audio
        allegro_acodec

        lisp
)

####################################################################
# Editor Executable

if (CHORIZO_BUILD_EDITOR)
    add_executable(edit WIN32
            ${ENGINE_SOURCE_FILES}
            ${GAME_SOURCE_FILES})
    target_compile_definitions(edit
            PRIVATE "EDITOR")
    target_link_libraries(edit
            plcore
            plgraphics
            allegro
            allegro_dialog
            lisp)
endif ()
