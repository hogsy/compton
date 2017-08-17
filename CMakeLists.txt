cmake_minimum_required(VERSION 3.6)
project(VCritters)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

# Set all of our output directories.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

file(GLOB SOURCE_FILES
        *.cpp *.c *.h
        engine/*.*
        game/*.*
        game/objects/*.*
        game/objects/creature/*.*
        )

add_executable(VCritters ${SOURCE_FILES} game/objects/names.h)

include_directories(${CMAKE_SOURCE_DIR}/platform/include/ ${CMAKE_SOURCE_DIR}/external/OpenNN-master/)

add_subdirectory(platform)
#add_subdirectory(external/OpenNN-master)
add_subdirectory(allegro-5.2.2.0)

set_target_properties(VCritters PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

target_link_libraries(VCritters
        platform X11 dl
        allegro allegro_main allegro_dialog allegro_font allegro_ttf allegro_image allegro_primitives allegro_audio allegro_acodec
        )