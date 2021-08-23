#file(GLOB_RECURSE sources "*.cpp" "*.hpp" "*.h" "*.c")
CM_GetAllSources(sources)

set(EXAMPLE_NAME ${PROJECT_NAME}_example)

add_executable(${EXAMPLE_NAME} ${sources})
target_link_libraries(${EXAMPLE_NAME} ${SOUND_PROCESSOR_LIBRARIES})
target_include_directories(${EXAMPLE_NAME} PRIVATE ${SOUND_PROCESSOR_INCLUDES})

# Copy resource directory to output directory
add_custom_command(TARGET ${EXAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SOUND_PROCESSOR_RESOURCE_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SOUND_PROCESSOR_RESOURCE_DIR_NAME})
