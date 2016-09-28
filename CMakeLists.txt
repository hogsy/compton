cmake_minimum_required(VERSION 3.6)
project(VCritters)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set all of our output directories.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")

set(SOURCE_FILES engine.cpp game.cpp math.cpp math.h alife.cpp alife.h sky_manager.cpp sky_manager.h game.h)
add_executable(VCritters ${SOURCE_FILES})

set_target_properties(VCritters PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)

target_link_libraries(VCritters allegro allegro_dialog allegro_font allegro_ttf allegro_image allegro_primitives)