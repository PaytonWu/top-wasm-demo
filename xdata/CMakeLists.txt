aux_source_directory(./src src_dir)
add_library(xdata STATIC ${src_dir})

add_dependencies(xdata xconfig xcommon xbasic)
target_link_libraries(xdata PRIVATE xconfig xcommon xbasic)
