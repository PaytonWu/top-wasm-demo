aux_source_directory(./src src_dir)
add_library(xstate_accessor STATIC ${src_dir})

add_dependencies(xstate_accessor xdata xcommon xconfig xbasic)
target_link_libraries(xstate_accessor PRIVATE xdata xcommon xconfig xbasic)
