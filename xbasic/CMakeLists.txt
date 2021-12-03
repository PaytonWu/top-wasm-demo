aux_source_directory(./src xbasic_src)
add_library(xbasic STATIC ${xbasic_src})

target_link_libraries(xbasic PRIVATE msgpackc)

