file(GLOB_RECURSE sources "*.cpp" "*.hpp" "*.h" "*.c")

add_library(${PROJECT_NAME} STATIC ${sources})
target_link_libraries(${PROJECT_NAME} ${SOUND_PROCESSOR_LIBRARIES_PRIVATE})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOUND_PROCESSOR_INCLUDES})