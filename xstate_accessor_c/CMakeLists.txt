aux_source_directory(./src xsac_src)

add_library(xstate_accessor_c STATIC ${xsac_src})
add_dependencies(xstate_accessor_c xstate_accessor)
target_link_libraries(xstate_accessor_c PRIVATE xstate_accessor)