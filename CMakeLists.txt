cmake_minimum_required(VERSION 3.17)
project(SoundProcessor)

include(cmake/CPM.cmake)
include(cmake/CMUtils.cmake)

CPMAddPackage("gh:jarro2783/cxxopts@2.2.1")
CPMAddPackage("gh:gabime/spdlog@1.9.2")
CPMAddPackage("gh:catchorg/Catch2@2.5.0")
CPMAddPackage("gh:kcat/openal-soft#1.21.1")
CPMAddPackage("gh:fmtlib/fmt#8.0.0")
CPMAddPackage("gh:ocornut/imgui#v1.83")

CM_Setup(20)

# Libraries
set(SOUND_PROCESSOR_LIBRARIES_PRIVATE
        cxxopts
        spdlog
        openal)
set(SOUND_PROCESSOR_LIBRARIES 
    ${PROJECT_NAME}
    ${SOUND_PROCESSOR_LIBRARIES_PRIVATE})

# Includes
set(SOUND_PROCESSOR_INCLUDES ${CMAKE_SOURCE_DIR}/lib/include)

# Resources
set(SOUND_PROCESSOR_RESOURCE_DIR_NAME res)
set(SOUND_PROCESSOR_RESOURCE_DIR ${CMAKE_SOURCE_DIR}/${SOUND_PROCESSOR_RESOURCE_DIR_NAME}/)

# Subprojects
add_subdirectory(3rdparty)
add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(example)
