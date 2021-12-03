aux_source_directory(./src src_dir)

add_library(xconfig STATIC ${src_dir})
target_link_libraries(xconfig PRIVATE xbasic)
