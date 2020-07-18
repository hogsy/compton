cmake_minimum_required(VERSION 3.6)

# Set all of our output directories.
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/lib/")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

add_subdirectory( platform/platform/ )
add_subdirectory( src/tools/pkgman/ )

project( VCritters )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

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

add_definitions( -DGIT_COMMIT_HASH="${GIT_COMMIT_HASH}" )
add_definitions( -DGIT_COMMIT_COUNT="${GIT_COMMIT_COUNT}" )
add_definitions( -DGIT_BRANCH="${GIT_BRANCH}" )

file(GLOB SOURCE_FILES
        src/shared.h

        src/engine/*.cpp
        src/engine/*.h

        src/game/*.cpp
        src/game/*.h
        )

add_executable( VCritters ${SOURCE_FILES}
        # engine
        src/game/SpriteSheet.cpp
        src/game/SpriteSheet.h
        src/engine/GUIPanel.cpp
        src/engine/GUIPanel.h
        src/engine/SimGame.h
        src/engine/ScriptParser.cpp
        src/engine/ScriptParser.h
        src/engine/LoaderGfx.cpp
        src/engine/LoaderPkg.cpp
        src/game/StateStorage.cpp
        src/game/StateStorage.h

        src/game/agents/Cloud.cpp
        src/game/agents/PhysicsAgent.cpp
        src/game/agents/SpriteAgent.cpp
        )

set_target_properties( VCritters PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/ )

target_include_directories( VCritters PRIVATE src/engine/ src/game/ )
if ( WIN32 )
    target_include_directories( VCritters PRIVATE src/3rdparty/allegro/include/ )
    target_link_directories( VCritters PRIVATE src/3rdparty/allegro/lib/ )
    target_link_libraries( VCritters -Wl,-Bstatic stdc++ winpthread -Wl,-Bdynamic -static-libstdc++ -static-libgcc )
endif( WIN32 )
target_link_libraries( VCritters
        platform
        allegro
        allegro_dialog
        allegro_font
        allegro_ttf
        allegro_primitives
        allegro_audio
        allegro_acodec
        )