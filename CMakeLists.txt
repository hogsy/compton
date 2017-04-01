cmake_minimum_required(VERSION 3.6)
project(VCritters)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set all of our output directories.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

file(GLOB SOURCE_FILES
        *.cpp *.c *.h
        engine/*.*
        game/*.*
        game/objects/*.*
        )

add_executable(VCritters ${SOURCE_FILES})

include_directories(${CMAKE_SOURCE_DIR}/platform/include/ ${CMAKE_SOURCE_DIR}/external/OpenNN-master/)

add_subdirectory(platform)
add_subdirectory(external/OpenNN-master)

set_target_properties(VCritters PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

target_link_libraries(VCritters platform X11 dl opennn allegro allegro_dialog allegro_font allegro_ttf allegro_image allegro_primitives)