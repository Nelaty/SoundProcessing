file(GLOB_RECURSE sources "*.cpp" "*.hpp" "*.h" "*.c")

set(TEST_NAME ${PROJECT_NAME}_test)

add_executable(${TEST_NAME} ${sources})
target_link_libraries(${TEST_NAME} ${SOUND_PROCESSOR_LIBRARIES})
target_include_directories(${TEST_NAME} PRIVATE ${SOUND_PROCESSOR_INCLUDES} include)

# Copy resource directory to output directory
add_custom_command(TARGET ${TEST_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SOUND_PROCESSOR_RESOURCE_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SOUND_PROCESSOR_RESOURCE_DIR_NAME})
