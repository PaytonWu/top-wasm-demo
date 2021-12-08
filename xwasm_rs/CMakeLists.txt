find_program(CARGO cargo)
set(ARGS --target-dir ${CMAKE_BINARY_DIR})

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(ARGS ${ARGS} --release)
endif()

add_custom_target(xwasm_rs ALL COMMAND ${CMAKE_COMMAND} -E environment COMMAND ${CARGO} build ${ARGS} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})